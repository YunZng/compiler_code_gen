#include <cassert>
#include "node.h"
#include "instruction.h"
#include "highlevel.h"
#include "ast.h"
#include "parse.tab.h"
#include "grammar_symbols.h"
#include "exceptions.h"
#include "highlevel_codegen.h"
#include <algorithm>

namespace{

  // Adjust an opcode for a basic type
  HighLevelOpcode get_opcode(HighLevelOpcode base_opcode , const std::shared_ptr<Type>& type){
    if (type->is_basic())
      return static_cast<HighLevelOpcode>(int(base_opcode) + int(type->get_basic_type_kind()));
    else if (type->is_pointer())
      return static_cast<HighLevelOpcode>(int(base_opcode) + int(BasicTypeKind::LONG));
    else
      RuntimeError::raise("attempt to use type '%s' as data in opcode selection" , type->as_str().c_str());
  }

}

HighLevelCodegen::HighLevelCodegen(int next_label_num)
  : m_next_label_num(next_label_num)
  , m_hl_iseq(new InstructionSequence()){
  curVreg = 10;
  argVreg = 1;
  highestVreg = 0;
}

HighLevelCodegen::~HighLevelCodegen(){
}

int debugs = 0;
void HighLevelCodegen::visit_function_definition(Node* n){
  printf("%s" , debugs ? "hc visit_function_definition\n" : "");
  // generate the name of the label that return instructions should target
  curVreg = n->get_vreg();
  imVreg = curVreg;
  std::string fn_name = n->get_kid(1)->get_str();
  m_return_label_name = ".L" + fn_name + "_return";

  unsigned total_local_storage = n->get_symbol()->get_addr();
  /*
    total_local_storage = n->get_total_local_storage();
  */

  m_hl_iseq->append(new Instruction(HINS_enter , Operand(Operand::IMM_IVAL , total_local_storage)));
  //pass param
  Node* param_list = n->get_kid(2);
  for (int i = 0; i < param_list->get_num_kids(); i++){
    Node* param = param_list->get_kid(i);
    visit_variable_ref(param);
    Operand first = param->get_op();
    Operand second(Operand::VREG , argVreg++);
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b , param->get_type()) , first , second));
  }
  // reset arg register
  argVreg = 1;
  // visit body
  visit(n->get_kid(3));

  m_hl_iseq->define_label(m_return_label_name);
  m_hl_iseq->append(new Instruction(HINS_leave , Operand(Operand::IMM_IVAL , total_local_storage)));
  m_hl_iseq->append(new Instruction(HINS_ret));


  n->get_symbol()->set_addr(total_local_storage + (highestVreg - 10) * 8);
  // printf("total vreg %d\n" , highestVreg);
}

void HighLevelCodegen::visit_return_statement(Node* n){
  printf("%s" , debugs ? "hc visit_return_statement\n" : "");
  // jump to the return label
  m_hl_iseq->append(new Instruction(HINS_jmp , Operand(Operand::LABEL , m_return_label_name)));
}

void HighLevelCodegen::visit_return_expression_statement(Node* n){
  printf("%s" , debugs ? "hc visit_return_expression_statement\n" : "");
  // A possible implementation:
  Node* expr = n->get_kid(0);

  // generate code to evaluate the expression
  visit(expr);

  // move the computed value to the return value vreg
  HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b , expr->get_type());
  m_hl_iseq->append(new Instruction(mov_opcode , Operand(Operand::VREG , 0) , expr->get_op()));

  // jump to the return label
  visit_return_statement(n);
}

void HighLevelCodegen::visit_while_statement(Node* n){
  printf("%s" , debugs ? "hc visit_while_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  std::string cond_label = ".L" + std::to_string(get_next_label_num());

  m_hl_iseq->append(new Instruction(HINS_jmp , Operand(Operand::LABEL , cond_label)));
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(1));
  m_hl_iseq->define_label(cond_label);
  visit(n->get_kid(0));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t , n->get_kid(0)->get_op() , Operand(Operand::LABEL , body_label)));
}

void HighLevelCodegen::visit_do_while_statement(Node* n){
  printf("%s" , debugs ? "hc visit_do_while_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(0));
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t , Operand(Operand::VREG , curVreg) , Operand(Operand::LABEL , body_label)));
}

void HighLevelCodegen::visit_for_statement(Node* n){
  printf("%s" , debugs ? "hc visit_for_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  std::string cond_label = ".L" + std::to_string(get_next_label_num());

  visit(n->get_kid(0));
  m_hl_iseq->append(new Instruction(HINS_jmp , Operand(Operand::LABEL , cond_label)));
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(3));
  visit(n->get_kid(2));
  m_hl_iseq->define_label(cond_label);
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t , Operand(Operand::VREG , curVreg) , Operand(Operand::LABEL , body_label)));
}

void HighLevelCodegen::visit_if_statement(Node* n){
  printf("%s" , debugs ? "hc visit_if_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  // cond
  visit(n->get_kid(0));
  // if false, don't reach body, jump to label
  m_hl_iseq->append(new Instruction(HINS_cjmp_f , Operand(Operand::VREG , curVreg) , Operand(Operand::LABEL , body_label)));
  // body
  visit(n->get_kid(1));
  m_hl_iseq->define_label(body_label);
}

void HighLevelCodegen::visit_if_else_statement(Node* n){
  printf("%s" , debugs ? "hc visit_if_else_statement\n" : "");
  std::string after_else_label = ".L" + std::to_string(get_next_label_num());
  std::string else_label = ".L" + std::to_string(get_next_label_num());
  // cond
  visit(n->get_kid(0));
  // if false, jump to label for else
  m_hl_iseq->append(new Instruction(HINS_cjmp_f , Operand(Operand::VREG , curVreg) , Operand(Operand::LABEL , else_label)));
  // true body
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_jmp , Operand(Operand::LABEL , after_else_label)));
  // else body
  m_hl_iseq->define_label(else_label);
  visit(n->get_kid(2));

  m_hl_iseq->define_label(after_else_label);
}

void HighLevelCodegen::visit_binary_expression(Node* n){
  printf("%s" , debugs ? "hc visit_binary_expression\n" : "");
  visit(n->get_kid(1));
  visit(n->get_kid(2));
  Operand first = n->get_kid(1)->get_op();
  Operand second = n->get_kid(2)->get_op();

  HighLevelOpcode op_code;
  switch (n->get_kid(0)->get_tag()){
    case TOK_ASSIGN:{
      m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b , n->get_kid(1)->get_type()) , first , second));
      curVreg = imVreg;
      return;
    }
    case TOK_PLUS:{
      op_code = HINS_add_b;
      break;
    }
    case TOK_MINUS:{
      op_code = HINS_sub_b;
      break;
    }
    case TOK_ASTERISK:{
      op_code = HINS_mul_b;
      break;
    }
    case TOK_DIVIDE:{
      op_code = HINS_div_b;
      break;
    }
    case TOK_LT:{
      op_code = HINS_cmplt_b;
      break;
    }
    case TOK_LTE:{
      op_code = HINS_cmplte_b;
      break;
    }
    case TOK_GT:{
      op_code = HINS_cmpgt_b;
      break;
    }
    case TOK_GTE:{
      op_code = HINS_cmpgte_b;
      break;
    }
    case TOK_EQUALITY:{
      op_code = HINS_cmpeq_b;
      break;
    }
    case TOK_INEQUALITY:{
      op_code = HINS_cmpneq_b;
      break;
    }
    default: RuntimeError::raise("should not reach here");
  }
  if (first.is_memref()){
    Operand dest(next_vr());
    // if memref then auto _l since pointer is _l
    m_hl_iseq->append(new Instruction(HINS_mov_l , dest , first));
    first = dest;
  }
  if (second.is_memref()){
    Operand dest(next_vr());
    // if memref then auto _l since pointer is _l
    m_hl_iseq->append(new Instruction(HINS_mov_l , dest , second));
    second = dest;
  }
  Operand dest(next_vr());
  m_hl_iseq->append(new Instruction(get_opcode(op_code , n->get_kid(1)->get_type()) , dest , first , second));
  curVreg = imVreg;
  n->set_op(dest);
}

void HighLevelCodegen::visit_unary_expression(Node* n){
  printf("%s" , debugs ? "hc visit_unary_expression\n" : "");
  int tag = n->get_kid(0)->get_tag();
  int addr;
  Operand dest = Operand(Operand::VREG , curVreg);
  Operand first;
  Node* var = n->get_kid(1);
  switch (tag){
    case TOK_ASTERISK:{
      visit(var);
      // if p of *p is already a pointer and p->get_op is memref, then we need to do extra work to store memref somewhere
      // else if not memref, make it memref
      if (var->get_op().is_memref()){
        dest = next_vr();
        m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b , var->get_type()) , dest , var->get_op()));
        dest = dest.to_memref();
      } else{
        dest = var->get_op().to_memref();
      }

      break;
    }
    case TOK_AMPERSAND:{
      dest = next_vr();
      addr = var->get_symbol()->get_addr();
      first = Operand(Operand::IMM_IVAL , addr);
      m_hl_iseq->append(new Instruction(HINS_localaddr , dest , first));
      break;
    }
  }
  // curVreg = imVreg;
  n->set_op(dest);
}

void HighLevelCodegen::visit_function_call_expression(Node* n){
  printf("%s" , debugs ? "hc visit_function_call_expression\n" : "");
  //arg passing
  Node* arg_list = n->get_kid(1);
  for (int i = 0; i < arg_list->get_num_kids(); i++){
    Node* kid = arg_list->get_kid(i);
    visit(kid);
    Operand first = kid->get_op();
    Operand second(Operand::VREG , argVreg++);
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b , kid->get_type()) , second , first));
  }
  // reset arg register
  argVreg = 1;
  // call label
  m_hl_iseq->append(new Instruction(HINS_call , Operand(Operand::LABEL , n->get_str())));
  // return val
  if (n->get_type()->get_basic_type_kind() != BasicTypeKind::VOID){
    Operand rax = next_vr();
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b , n->get_type()) , rax , Operand(Operand::VREG , 0)));
    n->set_op(rax);
  }
}

void HighLevelCodegen::visit_array_element_ref_expression(Node* n){
  printf("%s" , debugs ? "hc visit_array_element_ref_expression\n" : "");
  Node* arr = n->get_kid(0);
  Node* index = n->get_kid(1);
  //if array allocated, size will be ≥0
  int addr = arr->get_symbol()->get_addr();
  Operand dest;
  Operand first(Operand::IMM_IVAL , addr);
  //size of each array element
  Operand second(Operand::IMM_IVAL , arr->get_type()->get_base_type()->get_alignment());
  Operand start_addr(Operand::VREG , arr->get_symbol()->get_vreg());
  //localaddr vr11, $0
  //This part is not necessary for array in a function call, since we don't know the address before hand
  if (addr != -1){
    dest = next_vr();
    m_hl_iseq->append(new Instruction(HINS_localaddr , dest , first));
    start_addr = dest;
  }
  //find offset (mul_b offset index siz)
  visit(index);
  dest = next_vr();
  first = index->get_op();
  std::shared_ptr<Type> index_type = index->get_type();
  HighLevelOpcode code = HINS_nop;
  switch (index_type->get_basic_type_kind()){
    case BasicTypeKind::CHAR:{
      code = HINS_uconv_bq;
      if (index_type->is_signed()){
        code = HINS_sconv_bq;
      }
      break;
    }
    case BasicTypeKind::SHORT:{
      code = HINS_uconv_wq;
      if (index_type->is_signed()){
        code = HINS_sconv_wq;
      }
      break;
    }
    case BasicTypeKind::INT:{
      code = HINS_uconv_lq;
      if (index_type->is_signed()){
        code = HINS_sconv_lq;
      }
      break;
    }
    default: break;
  }
  if (code != HINS_nop){
    m_hl_iseq->append(new Instruction(code , dest , first));
    first = dest;
    dest = next_vr();
  }
  m_hl_iseq->append(new Instruction(get_opcode(HINS_mul_b , arr->get_type()) , dest , first , second));
  //adjust address with offset
  second = dest;
  dest = next_vr();
  m_hl_iseq->append(new Instruction(get_opcode(HINS_add_b , arr->get_type()) , dest , start_addr , second));
  //memory dereference
  n->set_op(dest.to_memref());
}

void HighLevelCodegen::visit_variable_ref(Node* n){
  printf("%s" , debugs ? "hc visit_variable_ref\n" : "");
  Operand op;
  // create operand for variables NOT allocated in memory
  // otherwise, get its address into a register
  if (n->get_symbol()->get_addr() == -1){
    op = Operand(Operand::VREG , n->get_symbol()->get_vreg());
  } else{
    int addr = n->get_symbol()->get_addr();
    Operand first(Operand::IMM_IVAL , addr);
    op = next_vr();
    m_hl_iseq->append(new Instruction(HINS_localaddr , op , first));
    if (!n->get_type()->is_array()){
      op = op.to_memref();
    }
  }
  n->set_op(op);
}

void HighLevelCodegen::visit_field_ref_expression(Node* n){
  printf("%s" , debugs ? "hc visit_field_ref_expression\n" : "");
  std::string field_name = n->get_kid(1)->get_str();
  Node* strt = n->get_kid(0);
  std::shared_ptr<Type> var_type = strt->get_type();
  Operand dest(next_vr());
  Operand addr(Operand::IMM_IVAL , strt->get_symbol()->get_addr());
  Operand first;

  //First load address, later adjust with offset
  m_hl_iseq->append(new Instruction(HINS_localaddr , dest , addr));
  addr = dest;

  int offset = get_offset(var_type , field_name);
  dest = next_vr();
  first = Operand(Operand::IMM_IVAL , offset);
  //adjust addr with offset
  //offset is always signed int imm_ival, so auto the following are all _q
  m_hl_iseq->append(new Instruction(HINS_mov_q , dest , first));
  first = dest;
  m_hl_iseq->append(new Instruction(HINS_add_q , dest , addr , first));
  n->set_op(dest.to_memref());
}

void HighLevelCodegen::visit_indirect_field_ref_expression(Node* n){
  printf("%s" , debugs ? "hc visit_indirect_field_ref_expression\n" : "");
  Node* strt = n->get_kid(0);
  std::string field_name = n->get_kid(1)->get_str();
  //make sure dereference by get_base_type
  int offset = get_offset(strt->get_type()->get_base_type() , field_name);
  visit(strt);
  Operand addr = strt->get_op();
  Operand dest(next_vr());
  Operand first(Operand::IMM_IVAL , offset);
  m_hl_iseq->append(new Instruction(HINS_mov_q , dest , first));
  first = dest;
  m_hl_iseq->append(new Instruction(HINS_add_q , dest , addr , first));

  n->set_op(dest.to_memref());
}

void HighLevelCodegen::visit_literal_value(Node* n){
  printf("%s" , debugs ? "hc visit_literal_value\n" : "");
  // A partial implementation (note that this won't work correctly
  // for string constants!):
  LiteralValue val = n->get_type()->get_lit();
  Operand dest(next_vr());
  if (val.get_kind() == LiteralValueKind::INTEGER){
    HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b , n->get_type());
    m_hl_iseq->append(new Instruction(mov_opcode , dest , Operand(Operand::IMM_IVAL , val.get_int_value())));
    n->set_op(dest);
  } else if (val.get_kind() == LiteralValueKind::STRING){
    HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b , n->get_type());
    std::string str_label = "_str" + std::to_string(n->get_vreg());
    m_hl_iseq->append(new Instruction(mov_opcode , dest , Operand(Operand::IMM_LABEL , str_label)));
    n->set_op(dest);
  } else{
    RuntimeError::raise("Shoudl not be reached. Please don't take more credits away from me please, hope you have a good day");
  }
}

std::string HighLevelCodegen::next_label(){
  std::string label = ".L" + std::to_string(m_next_label_num++);
  return label;
}

Operand HighLevelCodegen::next_vr(){
  Operand a = Operand(Operand::VREG , curVreg++);
  highestVreg = std::max(highestVreg , curVreg);
  return a;
}

//trynna get offset
int HighLevelCodegen::get_offset(std::shared_ptr<Type> var_type , std::string field_name){
  int offset = 0;
  for (int i = 0; i < var_type->get_num_members(); i++){
    Member mbr = var_type->get_member(i);
    if (mbr.get_name() == field_name){
      break;
    }
    offset += mbr.get_type()->get_storage_size();
  }
  return offset;
}

