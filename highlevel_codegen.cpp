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
  HighLevelOpcode get_opcode(HighLevelOpcode base_opcode, const std::shared_ptr<Type>& type){
    if(type->is_basic()){
      return static_cast<HighLevelOpcode>(int(base_opcode) + int(type->get_basic_type_kind()));
    } else if(type->is_pointer()){
      return static_cast<HighLevelOpcode>(int(base_opcode) + int(BasicTypeKind::LONG));
    } else{
      RuntimeError::raise("attempt to use type '%s' as data in opcode selection", type->as_str().c_str());
    }
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
  printf("%s", debugs ? "hc visit_function_definition\n" : "");
  // generate the name of the label that return instructions should target
  curVreg = n->get_vreg();
  imVreg = curVreg;
  std::string fn_name = n->get_kid(1)->get_str();
  m_return_label_name = ".L" + fn_name + "_return";
  // puts("1jk");
  unsigned total_local_storage = n->get_symbol()->get_addr();
  /*
    total_local_storage = n->get_total_local_storage();
  */

  m_hl_iseq->append(new Instruction(HINS_enter, Operand(Operand::IMM_IVAL, total_local_storage)));
  //pass param
  Node* param_list = n->get_kid(2);
  for(int i = 0; i < param_list->get_num_kids(); i++){
    Node* param = param_list->get_kid(i);
    visit_variable_ref(param);
    Operand first = param->get_op();
    Operand second(Operand::VREG, argVreg++);
    // puts("9jk");
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, param->get_type()), first, second));
  }
  // reset arg register
  argVreg = 1;
  // visit body
  visit(n->get_kid(3));

  m_hl_iseq->define_label(m_return_label_name);
  m_hl_iseq->append(new Instruction(HINS_leave, Operand(Operand::IMM_IVAL, total_local_storage)));
  m_hl_iseq->append(new Instruction(HINS_ret));


  n->get_symbol()->set_addr(total_local_storage);
  n->get_symbol()->set_vreg(highestVreg);
  // printf("total vreg %d\n" , highestVreg);
}

void HighLevelCodegen::visit_return_statement(Node* n){
  printf("%s", debugs ? "hc visit_return_statement\n" : "");
  // jump to the return label
  m_hl_iseq->append(new Instruction(HINS_jmp, Operand(Operand::LABEL, m_return_label_name)));
}

void HighLevelCodegen::visit_return_expression_statement(Node* n){
  printf("%s", debugs ? "hc visit_return_expression_statement\n" : "");
  // A possible implementation:
  Node* expr = n->get_kid(0);

  // generate code to evaluate the expression
  visit(expr);

  std::shared_ptr<Type> index_type = expr->get_type();
  if(are_same(index_type, n->get_type()))
    goto done;
  convert(n->get_type(), expr);
done:
  // move the computed value to the return value vreg
  HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b, n->get_type());
  m_hl_iseq->append(new Instruction(mov_opcode, Operand(Operand::VREG, 0), expr->get_op()));

  // jump to the return label
  visit_return_statement(n);
}

void HighLevelCodegen::visit_while_statement(Node* n){
  printf("%s", debugs ? "hc visit_while_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  std::string cond_label = ".L" + std::to_string(get_next_label_num());

  m_hl_iseq->append(new Instruction(HINS_jmp, Operand(Operand::LABEL, cond_label)));
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(1));
  m_hl_iseq->define_label(cond_label);
  visit(n->get_kid(0));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t, n->get_kid(0)->get_op(), Operand(Operand::LABEL, body_label)));
}

void HighLevelCodegen::visit_do_while_statement(Node* n){
  printf("%s", debugs ? "hc visit_do_while_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(0));
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t, Operand(Operand::VREG, curVreg), Operand(Operand::LABEL, body_label)));
}

void HighLevelCodegen::visit_for_statement(Node* n){
  printf("%s", debugs ? "hc visit_for_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  std::string cond_label = ".L" + std::to_string(get_next_label_num());

  visit(n->get_kid(0));
  m_hl_iseq->append(new Instruction(HINS_jmp, Operand(Operand::LABEL, cond_label)));
  m_hl_iseq->define_label(body_label);
  visit(n->get_kid(3));
  visit(n->get_kid(2));
  m_hl_iseq->define_label(cond_label);
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_cjmp_t, n->get_kid(1)->get_op(), Operand(Operand::LABEL, body_label)));
}

void HighLevelCodegen::visit_if_statement(Node* n){
  printf("%s", debugs ? "hc visit_if_statement\n" : "");
  std::string body_label = ".L" + std::to_string(get_next_label_num());
  // cond
  visit(n->get_kid(0));
  // if false, don't reach body, jump to label
  m_hl_iseq->append(new Instruction(HINS_cjmp_f, Operand(Operand::VREG, curVreg), Operand(Operand::LABEL, body_label)));
  // body
  visit(n->get_kid(1));
  m_hl_iseq->define_label(body_label);
}

void HighLevelCodegen::visit_if_else_statement(Node* n){
  printf("%s", debugs ? "hc visit_if_else_statement\n" : "");
  std::string after_else_label = ".L" + std::to_string(get_next_label_num());
  std::string else_label = ".L" + std::to_string(get_next_label_num());
  // cond
  visit(n->get_kid(0));
  // if false, jump to label for else
  m_hl_iseq->append(new Instruction(HINS_cjmp_f, Operand(Operand::VREG, curVreg), Operand(Operand::LABEL, else_label)));
  // true body
  visit(n->get_kid(1));
  m_hl_iseq->append(new Instruction(HINS_jmp, Operand(Operand::LABEL, after_else_label)));
  // else body
  m_hl_iseq->define_label(else_label);
  visit(n->get_kid(2));

  m_hl_iseq->define_label(after_else_label);
}

void HighLevelCodegen::visit_binary_expression(Node* n){
  printf("%s", debugs ? "hc visit_binary_expression\n" : "");
  visit(n->get_kid(1));
  visit(n->get_kid(2));
  Operand first = n->get_kid(1)->get_op();
  Operand second = n->get_kid(2)->get_op();

  HighLevelOpcode op_code;
  switch(n->get_kid(0)->get_tag()){
    case TOK_ASSIGN:{
      std::shared_ptr<Type> type1 = n->get_kid(1)->get_type();
      std::shared_ptr<Type> type2 = n->get_kid(2)->get_type();
      if(type1->is_array()){
        while(type1->is_array()){
          type1 = type1->get_base_type();
        }
        if(!is_convertible(type1, type2)){
          type1 = n->get_kid(1)->get_type();
        };
      }
      // printf("assigned %s\n", type1->as_str().c_str());

      if(n->get_kid(2)->get_actually_var()){
        Operand temp = next_vr();
        m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, type1), temp, second));
        second = temp;
      } else if(type2->is_array()){
        second = Operand(Operand::VREG, n->get_kid(2)->get_vreg());
      }
      m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, type1), first, second));
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
    case TOK_MOD:{
      op_code = HINS_mod_b;
      break;
    }
    default: RuntimeError::raise("should not reach here");
  }
  if(first.is_memref()){
    Operand dest(next_vr());
    // if memref then auto _l since pointer is _l
    m_hl_iseq->append(new Instruction(HINS_mov_l, dest, first));
    first = dest;
  }
  if(second.is_memref()){
    Operand dest(next_vr());
    // if memref then auto _l since pointer is _l
    m_hl_iseq->append(new Instruction(HINS_mov_l, dest, second));
    second = dest;
  }
  Operand dest(next_vr());
  m_hl_iseq->append(new Instruction(get_opcode(op_code, n->get_kid(1)->get_type()), dest, first, second));
  curVreg = imVreg;
  n->set_op(dest);
}

void HighLevelCodegen::visit_unary_expression(Node* n){
  printf("%s", debugs ? "hc visit_unary_expression\n" : "");
  int tag = n->get_kid(0)->get_tag();
  int addr;
  Operand dest = Operand(Operand::VREG, curVreg);
  Operand first;
  Node* var = n->get_kid(1);
  visit(var);
  switch(tag){
    case TOK_MINUS:{
      dest = next_vr();
      if(var->get_lit()){
        first = Operand(Operand::IMM_IVAL, var->get_lit()->get_int_value());
      } else{
        first = var->get_op();
      }
      m_hl_iseq->append(new Instruction(get_opcode(HINS_neg_b, var->get_type()), dest, first));
      break;
    }
    case TOK_ASTERISK:{
      visit(var);
      // if p of *p is already a pointer and p->get_op is memref, then we need to do extra work to store memref somewhere
      // else if not memref, make it memref
      if(var->get_op().is_memref()){
        dest = next_vr();
        m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, var->get_type()), dest, var->get_op()));
        dest = dest.to_memref();
      } else{
        dest = var->get_op().to_memref();
      }

      break;
    }
    case TOK_AMPERSAND:{
      dest = next_vr();
      addr = var->get_symbol()->get_addr();
      first = Operand(Operand::IMM_IVAL, addr);
      m_hl_iseq->append(new Instruction(HINS_localaddr, dest, first));
      break;
    }
  }
  // curVreg = imVreg;
  n->set_op(dest);
}

void HighLevelCodegen::visit_function_call_expression(Node* n){
  printf("%s", debugs ? "hc visit_function_call_expression\n" : "");
  //arg passing
  Node* arg_list = n->get_kid(1);
  std::shared_ptr<Type> func = n->get_func();
  for(int i = 0; i < arg_list->get_num_kids(); i++){
    Node* kid = arg_list->get_kid(i);
    visit(kid);
    // puts("312");
    std::shared_ptr<Type> index_type = kid->get_type();
    std::shared_ptr<Type> type1 = func->get_member(i).get_type();
    if(are_same(index_type, type1))
      goto done;
    convert(type1, kid);
  done:
    // puts("319");

    Operand first = kid->get_op();
    Operand second(Operand::VREG, argVreg++);
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, type1), second, first));
  }
  // reset arg register
  argVreg = 1;
  // call label
  m_hl_iseq->append(new Instruction(HINS_call, Operand(Operand::LABEL, n->get_str())));
  // return val
  if(n->get_type()->get_basic_type_kind() != BasicTypeKind::VOID){
    Operand rax = next_vr();
    m_hl_iseq->append(new Instruction(get_opcode(HINS_mov_b, n->get_type()), rax, Operand(Operand::VREG, 0)));
    n->set_op(rax);
  }
  curVreg = imVreg;
}

void HighLevelCodegen::visit_array_element_ref_expression(Node* n){
  printf("%s", debugs ? "hc visit_array_element_ref_expression\n" : "");
  Node* arr = n->get_kid(0);
  Node* index = n->get_kid(1);
  visit(arr);
  // printf("who are tf you again %s\n", arr->get_type()->as_str().c_str());
  // printf("n type %s\n", n->get_type()->as_str().c_str());
  // puts("done visit");
  //if array allocated, size will be ≥0
  int addr;
  Operand start_addr;
  if(arr->has_symbol()){
    addr = arr->get_symbol()->get_addr();
    start_addr = Operand(Operand::VREG, arr->get_symbol()->get_vreg());
    // printf("who are you %s\n", arr->get_symbol()->get_type()->as_str().c_str());
    // puts("fuck u");
  } else if(arr->get_tag() == AST_FIELD_REF_EXPRESSION){
    start_addr = Operand(Operand::VREG, arr->get_op().get_base_reg());
    // puts("no symbol wa");
    // printf("%s\n", arr->get_type()->as_str().c_str());
    // addr = arr->get_type()->get_storage_size();
    /*
    mov_l    vr14, $0
    sconv_lq vr15, vr14
    mul_q    vr16, vr15, $1
    add_q    vr17, vr13, vr16
    */
    // addr = 2222;
    // return;
  }
  visit(index);

  Operand dest;
  Operand first(Operand::IMM_IVAL, addr);
  //size of each array element
  auto temp = arr->get_type();
  // while(temp->get_base_type()->is_array()){
  //   // temp = temp->get_base_type();
  // }
  // printf("who are you %s\n", temp->get_base_type()->as_str().c_str());
  Operand second(Operand::IMM_IVAL, n->get_type()->get_storage_size());
  //localaddr vr11, $0
  //This part is not necessary for array in a function call, since we don't know the address before hand
  if(addr != -1 && !arr->get_tag() == AST_FIELD_REF_EXPRESSION){
    dest = next_vr();
    m_hl_iseq->append(new Instruction(HINS_localaddr, dest, first));
    start_addr = dest;
  }
  //find offset (mul_b offset index siz)
  dest = next_vr();
  first = index->get_op();
  std::shared_ptr<Type> index_type = index->get_type();
  HighLevelOpcode code = HINS_nop;
  switch(index_type->get_basic_type_kind()){
    case BasicTypeKind::CHAR:{
      code = HINS_uconv_bq;
      if(index_type->is_signed()){
        code = HINS_sconv_bq;
      }
      break;
    }
    case BasicTypeKind::SHORT:{
      code = HINS_uconv_wq;
      if(index_type->is_signed()){
        code = HINS_sconv_wq;
      }
      break;
    }
    case BasicTypeKind::INT:{
      code = HINS_uconv_lq;
      if(index_type->is_signed()){
        code = HINS_sconv_lq;
      }
      break;
    }
    default: break;
  }
  if(code != HINS_nop){
    m_hl_iseq->append(new Instruction(code, dest, first));
    first = dest;
    dest = next_vr();
  }
  m_hl_iseq->append(new Instruction(get_opcode(HINS_mul_b, arr->get_type()), dest, first, second));
  //adjust address with offset
  second = dest;
  dest = next_vr();
  if(arr->get_type()->is_array()){
    start_addr = Operand(Operand::VREG, arr->get_vreg());
  } else{
    n->set_actually_var(true);
  }
  m_hl_iseq->append(new Instruction(get_opcode(HINS_add_b, arr->get_type()), dest, start_addr, second));
  //memory dereference
  n->set_op(dest.to_memref());
  n->set_symbol(arr->get_symbol());
  n->set_type(arr->get_type());
  n->set_vreg(dest.get_base_reg());
}

void HighLevelCodegen::visit_variable_ref(Node* n){
  printf("%s", debugs ? "hc visit_variable_ref\n" : "");
  Operand op;
  // create operand for variables NOT allocated in memory
  // otherwise, get its address into a register
  if(n->get_symbol()->get_addr() == -1){
    op = Operand(Operand::VREG, n->get_symbol()->get_vreg());
  } else{
    int addr = n->get_symbol()->get_addr();
    Operand first(Operand::IMM_IVAL, addr);
    op = next_vr();
    m_hl_iseq->append(new Instruction(HINS_localaddr, op, first));
    if(!n->get_type()->is_array()){
      op = op.to_memref();
    }
  }

  // puts("4jk");
  n->set_op(op);
  n->set_type(n->get_symbol()->get_type());
  n->set_vreg(op.get_base_reg());
}

void HighLevelCodegen::visit_field_ref_expression(Node* n){
  printf("%s", debugs ? "hc visit_field_ref_expression\n" : "");
  std::string field_name = n->get_kid(1)->get_str();
  visit(n->get_kid(0));
  Node* strt = n->get_kid(0);
  std::shared_ptr<Type> var_type = strt->get_type();
  Operand dest;
  // puts("<1>");
  Operand addr(Operand::IMM_IVAL, strt->get_symbol()->get_addr());
  Operand zero(Operand::IMM_IVAL, 0);

  // puts("<3>");

  Operand first;

  //First load address, later adjust with offset
  // printf("var type %s\n", var_type->as_str().c_str());
  if(var_type->is_array()){
    var_type = var_type->get_base_type();
  }
  // printf("type to str: %s\n", var_type->as_str().c_str());
  int offset = get_offset(var_type, field_name);
  // puts("<2>");

  dest = next_vr();
  first = Operand(Operand::IMM_IVAL, offset);
  //adjust addr with offset
  //offset is always signed int imm_ival, so auto the following are all _q
  m_hl_iseq->append(new Instruction(HINS_mov_q, dest, first));
  first = dest;
  Operand last_reg(Operand::VREG, strt->get_vreg());
  m_hl_iseq->append(new Instruction(HINS_add_q, dest, last_reg, first));
  n->set_op(dest.to_memref());
  n->set_vreg(dest.get_base_reg());
}

void HighLevelCodegen::visit_indirect_field_ref_expression(Node* n){
  printf("%s", debugs ? "hc visit_indirect_field_ref_expression\n" : "");
  Node* strt = n->get_kid(0);
  std::string field_name = n->get_kid(1)->get_str();
  //make sure dereference by get_base_type
  int offset = get_offset(strt->get_type()->get_base_type(), field_name);
  visit(strt);
  Operand addr = strt->get_op();
  Operand dest(next_vr());
  Operand first(Operand::IMM_IVAL, offset);
  m_hl_iseq->append(new Instruction(HINS_mov_q, dest, first));
  first = dest;
  m_hl_iseq->append(new Instruction(HINS_add_q, dest, addr, first));

  n->set_op(dest.to_memref());
}

void HighLevelCodegen::visit_literal_value(Node* n){
  printf("%s", debugs ? "hc visit_literal_value\n" : "");
  // A partial implementation (note that this won't work correctly
  // for string constants!):
  LiteralValue val = n->get_type()->get_lit();
  Operand dest(next_vr());
  if(val.get_kind() == LiteralValueKind::INTEGER){
    HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b, n->get_type());
    m_hl_iseq->append(new Instruction(mov_opcode, dest, Operand(Operand::IMM_IVAL, val.get_int_value())));
    n->set_op(dest);
  } else if(val.get_kind() == LiteralValueKind::STRING){
    HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b, n->get_type());
    std::string str_label = "_str" + std::to_string(n->get_vreg());
    m_hl_iseq->append(new Instruction(mov_opcode, dest, Operand(Operand::IMM_LABEL, str_label)));
    n->set_op(dest);
  } else if(val.get_kind() == LiteralValueKind::CHARACTER){
    HighLevelOpcode mov_opcode = get_opcode(HINS_mov_b, n->get_type());
    m_hl_iseq->append(new Instruction(mov_opcode, dest, Operand(Operand::IMM_IVAL, val.get_char_value())));
    n->set_op(dest);
  } else{
    RuntimeError::raise("Should not be reached. Please don't take more credits away from me please, hope you have a good day");
  }
  // printf("the shit is %s\n ", n->get_type()->as_str().c_str());
}

std::string HighLevelCodegen::next_label(){
  std::string label = ".L" + std::to_string(m_next_label_num++);
  return label;
}

Operand HighLevelCodegen::next_vr(){
  highestVreg = std::max(highestVreg, curVreg);
  Operand a = Operand(Operand::VREG, curVreg++);
  return a;
}

//trynna get offset
int HighLevelCodegen::get_offset(std::shared_ptr<Type> var_type, std::string field_name){
  int offset = 0;
  for(int i = 0; i < var_type->get_num_members(); i++){
    Member mbr = var_type->get_member(i);
    if(mbr.get_name() == field_name){
      break;
    }
    offset += mbr.get_type()->get_storage_size();
  }
  return offset;
}

//convert from node2 to node1
void HighLevelCodegen::convert(std::shared_ptr<Type> type1, Node* node2){

  // Node* expr = n->get_kid(0);
  std::shared_ptr<Type> index_type = node2->get_type();
  int dif = (int)type1->get_basic_type_kind();
  HighLevelOpcode code = HINS_nop;

  switch(index_type->get_basic_type_kind()){
    case BasicTypeKind::CHAR:{
      if(index_type->is_signed()){
        dif += HINS_sconv_bw - 1;
      } else{
        dif += HINS_uconv_bw - 1;
      }
      break;
    }
    case BasicTypeKind::SHORT:{
      if(index_type->is_signed()){
        dif += HINS_sconv_wl - 1;
      } else{
        dif += HINS_uconv_wl - 1;
      }
      break;
    }
    case BasicTypeKind::INT:{
      if(index_type->is_signed()){
        dif += HINS_sconv_lq - 2;
      } else{
        dif += HINS_uconv_lq - 2;
      }
      break;
    }
    default: break;
  }
  code = (HighLevelOpcode)dif;

  if(code != HINS_nop){
    Operand temp = next_vr();
    m_hl_iseq->append(new Instruction(code, temp, node2->get_op()));
    node2->set_op(temp);
    curVreg--;
  }
}
bool HighLevelCodegen::are_same(std::shared_ptr<Type> type1, std::shared_ptr<Type> type2){
  if(type1->as_str() == type2->as_str()){
    return true;
  }
  if(type1->is_array() && type2->is_array()){
    return type1->is_same(type2.get());
  }
  if(type1->is_basic() && type2->is_basic()){
    return type1->is_same(type2.get());
  }
  if(type1->is_array() && type2->is_pointer()){
    return are_same(type1->get_base_type(), type2->get_base_type());
  }
  if(type2->is_array() && type1->is_pointer()){
    return are_same(type1->get_base_type(), type2->get_base_type());
  }
  if(type1->is_pointer() && type2->is_pointer()){
    return type1->is_same(type2.get());
  }
  if(type1->is_array() && !type2->is_array()){
    return are_same(type1->get_base_type(), type2);
    // return type1->get_base_type()->is_same(type2.get());
  }
  if(type2->is_array() && !type1->is_array()){
    return are_same(type2->get_base_type(), type1);

    // return type2->get_base_type()->is_same(type1.get());
  }
  return false;
}
bool HighLevelCodegen::is_convertible(std::shared_ptr<Type> l, std::shared_ptr<Type> r){
  if(l->is_same(r.get())){
    return true;
  }
  if(l->is_integral() && r->is_integral()){
    return true;
  }
  if(l->is_array() && r->is_array() && (is_convertible(l->get_base_type(), r->get_base_type()))){
    return true;
  }
  if(l->is_array() && r->is_pointer() && (is_convertible(l->get_base_type(), r->get_base_type()))){
    return true;
  }
  if(l->is_pointer() && r->is_array() && (is_convertible(l->get_base_type(), r->get_base_type()))){
    return true;
  }
  return false;
}