#include <cassert>
#include <algorithm>
#include <utility>
#include <map>
#include "grammar_symbols.h"
#include "parse.tab.h"
#include "node.h"
#include "ast.h"
#include "exceptions.h"
#include "semantic_analysis.h"

SemanticAnalysis::SemanticAnalysis()
  : m_global_symtab(new SymbolTable(nullptr)){
  m_cur_symtab = m_global_symtab;
}

SemanticAnalysis::~SemanticAnalysis(){
  delete(m_cur_symtab);
}

SymbolTable* SemanticAnalysis::get_global_symtab(){
  return m_global_symtab;
}

int debug = 0;
void SemanticAnalysis::visit_struct_type(Node* n){
  if(debug){ puts("visit_struct_type"); }
  std::string name = "struct " + n->get_kid(0)->get_str();
  Symbol* target = m_cur_symtab->lookup_recursive_kind(name, SymbolKind::TYPE);
  if(!target){
    SemanticError::raise(n->get_loc(), "No such struct visit_struct_type");
  }
  n->set_type(target->get_type());
}

void SemanticAnalysis::visit_union_type(Node* n){
  RuntimeError::raise("union types aren't supported");
}

void SemanticAnalysis::visit_variable_declaration(Node* n){
  if(debug){ puts("visit_variable_declaration"); }
  //kid 0 = storage
  //kid 1 = basic type(char, long, short, int, void, signed, unsigned) list
  //kid 2 = declarator(optional pointer + var name) list
  //construct complete type rep
  visit(n->get_kid(1));
  std::shared_ptr<Type> base_type = n->get_kid(1)->get_type();
  Node* decl_list = n->get_kid(2);
  // for each 
  for(auto i = decl_list->cbegin(); i != decl_list->cend(); ++i){
    Node* declarator = *i;
    std::shared_ptr<Type> base_type1 = base_type;
    std::string name = build_type(declarator, base_type1);
    if(m_cur_symtab->has_symbol_local(name)){
      SemanticError::raise(declarator->get_kid(0)->get_loc(), "Already defined");
    }

    declarator->set_type(base_type1);
    declarator->set_symbol(m_cur_symtab->declare(SymbolKind::VARIABLE, name, base_type1));
    declarator->set_str(name);
  }
}

void SemanticAnalysis::visit_basic_type(Node* n){
  if(debug){ puts("visit_basic_type"); }
  int kid_cnt = n->get_num_kids();
  // Flags for type creation
  int is_signed = -1;
  TypeQualifier is_const = TypeQualifier::NOTHING;
  TypeQualifier is_volatile = TypeQualifier::NOTHING;
  BasicTypeKind type_kind = BasicTypeKind::NOTHING;
  // Go through all kids to set flags
  for(int i = 0; i < kid_cnt; i++){
    switch(n->get_kid(i)->get_tag()){
      case TOK_SIGNED:{
        if(is_signed == -1){
          is_signed = 1;
        } else{
          SemanticError::raise(n->get_loc(), "Too many signed/unsigned");
        }
        break;
      }
      case TOK_UNSIGNED:{
        if(is_signed == -1){
          is_signed = 0;
        } else{
          SemanticError::raise(n->get_loc(), "Too many signed/unsigned");
        }
        break;
      }
      case TOK_CHAR:{
        if(type_kind == BasicTypeKind::NOTHING){
          type_kind = BasicTypeKind::CHAR;
        } else{
          SemanticError::raise(n->get_loc(), "Cannot be more than char at a time");
        }
        break;
      }
      case TOK_SHORT:{
        if(type_kind == BasicTypeKind::NOTHING || type_kind == BasicTypeKind::INT){
          type_kind = BasicTypeKind::SHORT;
        } else{
          SemanticError::raise(n->get_loc(), "Cannot be more than short at a time");
        }
        break;
      }
      case TOK_INT:{
        if(type_kind == BasicTypeKind::NOTHING){
          type_kind = BasicTypeKind::INT;
        } else if(type_kind == BasicTypeKind::SHORT || type_kind == BasicTypeKind::LONG){
        } else{
          SemanticError::raise(n->get_loc(), "Cannot be more than int at a time");
        }
        break;
      }
      case TOK_LONG:{
        if(type_kind == BasicTypeKind::NOTHING || type_kind == BasicTypeKind::INT){
          type_kind = BasicTypeKind::LONG;
        } else{
          SemanticError::raise(n->get_loc(), "Cannot be more than long at a time");
        }
        break;
      }
      case TOK_VOID:{
        if(type_kind == BasicTypeKind::NOTHING){
          type_kind = BasicTypeKind::VOID;
        } else{
          SemanticError::raise(n->get_loc(), "Cannot be more than void at a time");
        }
        break;
      }
      case TOK_CONST:{
        if(is_const == TypeQualifier::NOTHING){
          is_const = TypeQualifier::CONST;
        } else{
          SemanticError::raise(n->get_loc(), "Too many const");
        }
        break;
      }
      case TOK_VOLATILE:{
        if(is_volatile == TypeQualifier::NOTHING){
          is_volatile = TypeQualifier::VOLATILE;
        } else{
          SemanticError::raise(n->get_loc(), "Too many volatile");
        }
        break;
      }
      default:{break;}
    }
  }

  // Final basic type checking
  if(type_kind == BasicTypeKind::NOTHING){
    type_kind = BasicTypeKind::INT;
  }
  if(type_kind == BasicTypeKind::VOID && (is_signed != -1 || is_const != TypeQualifier::NOTHING || is_volatile != TypeQualifier::NOTHING)){
    SemanticError::raise(n->get_loc(), "Void cannot be signed/unsigned and(or) qualified");
  }

  // Basic type creation
  std::shared_ptr<Type> type_temp(new BasicType(type_kind, is_signed));

  //Qualified type creation (if specified)
  if(is_volatile != TypeQualifier::NOTHING){
    std::shared_ptr<Type> temp(new QualifiedType(type_temp, is_volatile));
    type_temp = temp;
  }
  if(is_const != TypeQualifier::NOTHING){
    std::shared_ptr<Type> temp(new QualifiedType(type_temp, is_const));
    type_temp = temp;
  }

  // Pass result using node_base::set_type
  n->set_type(type_temp);
}

void SemanticAnalysis::visit_function_definition(Node* n){
  if(debug){ puts("visit_function_definition"); }
  // get basic type 
  visit(n->get_kid(0));
  std::shared_ptr<Type> func_type(new FunctionType(n->get_kid(0)->get_type()));

  // get name
  std::string name = n->get_kid(1)->get_str();

  // get parameter list
  Node* param_list = n->get_kid(2);
  // add each parameter to func type
  for(auto i = param_list->cbegin(); i != param_list->cend(); ++i){
    Node* param = *i;
    visit(param);
    std::string param_name = param->get_kid(1)->get_str();
    func_type->add_member(Member(param_name, param->get_kid(1)->get_type()));
  }

  // Sanity check for function
  if(m_cur_symtab->has_symbol_local(name)){
    if(m_cur_symtab->lookup_local(name)->is_defined()){
      SemanticError::raise(n->get_loc(), "Cannot redefine function %s", name.c_str());
    } else{
      m_cur_symtab->lookup_local(name)->set_is_defined(true);
      leave_scope();
      return;
    }
  }
  n->set_symbol(m_cur_symtab->define(SymbolKind::FUNCTION, name, func_type));

  // new scope for func param, and add them to table, apparently order matters
  enter_scope();
  for(auto i = param_list->cbegin(); i != param_list->cend(); ++i){
    Node* param = *i;
    std::string param_name = param->get_kid(1)->get_str();
    if(m_cur_symtab->has_symbol_local(param_name)){
      SemanticError::raise(param->get_kid(1)->get_loc(), "Cannot have duplicate names");
    }
    param->set_symbol(m_cur_symtab->declare(SymbolKind::VARIABLE, param_name, param->get_kid(1)->get_type()));
  }
  m_cur_symtab->set_fn_type(func_type);
  enter_scope();
  visit(n->get_kid(3));
  leave_scope();
  leave_scope();
}

void SemanticAnalysis::visit_function_declaration(Node* n){
  if(debug){ puts("visit_function_declaration"); }
  visit(n->get_kid(0));
  std::shared_ptr<Type> func_type(new FunctionType(n->get_kid(0)->get_type()));

  // get name
  std::string name = n->get_kid(1)->get_str();

  // get parameter list
  Node* param_list = n->get_kid(2);

  // add each parameter to func type
  for(auto i = param_list->cbegin(); i != param_list->cend(); ++i){
    Node* param = *i;
    visit(param);
    std::string param_name = param->get_kid(1)->get_str();
    func_type->add_member(Member(param_name, param->get_kid(1)->get_type()));
  }

  // Func type sanity check
  if(m_cur_symtab->has_symbol_local(name)){
    SemanticError::raise(n->get_loc(), "Cannot redeclaration function %s", name.c_str());
  }
  m_cur_symtab->declare(SymbolKind::FUNCTION, name, func_type);
}

void SemanticAnalysis::visit_function_parameter(Node* n){
  if(debug){ puts("visit_function_parameter"); }
  visit(n->get_kid(0));
  std::shared_ptr<Type> base_type = n->get_kid(0)->get_type();
  std::shared_ptr<Type> base_type1 = base_type;
  std::string name = build_type(n->get_kid(1), base_type1);
  n->get_kid(1)->set_type(base_type1);
  n->get_kid(1)->set_str(name);
}

void SemanticAnalysis::visit_statement_list(Node* n){
  if(debug){ puts("visit_statement_list"); }
  visit_children(n);
}

void SemanticAnalysis::visit_struct_type_definition(Node* n){
  if(debug){ puts("visit_struct_type_definition"); }

  //get name
  std::string name = n->get_kid(0)->get_str();
  if(m_cur_symtab->has_symbol_local("struct " + name)){
    SemanticError::raise(n->get_loc(), "Cannot redefine struct %s", name.c_str());
  }
  std::shared_ptr<Type> s_type(new StructType(name));
  m_cur_symtab->define(SymbolKind::TYPE, "struct " + name, s_type);
  std::vector<Member> m_vector;
  //AST_FIELD_DEFINITION_LIST
  enter_scope();
  Node* decl_list = n->get_kid(1);
  for(auto i = decl_list->cbegin(); i != decl_list->cend(); ++i){
    Node* ast_var_dec = *i;
    visit(ast_var_dec);
    //AST_DECLARATOR_LIST
    Node* decl_list1 = ast_var_dec->get_kid(2);
    for(auto i = decl_list1->cbegin(); i != decl_list1->cend(); ++i){
      Node* declarator = *i;
      m_vector.push_back(Member(declarator->get_str(), declarator->get_type()));
    }
  }
  for(auto i : m_vector){
    s_type->add_member(i);
  }
  leave_scope();
}

void SemanticAnalysis::visit_binary_expression(Node* n){
  if(debug){ puts("visit_binary_expression"); }

  //Operator
  int tag = n->get_kid(0)->get_tag();
  //lvalue
  visit(n->get_kid(1));
  std::shared_ptr<Type> l_type = n->get_kid(1)->get_type();
  std::string l_name = n->get_kid(1)->get_str();
  //rvalue
  visit(n->get_kid(2));
  std::shared_ptr<Type> r_type = n->get_kid(2)->get_type();

  switch(tag){
    //For assignment
    case TOK_ASSIGN:{
      //check if lvalue
      // printf("%s\n", l_type->as_str().c_str());
      if(!l_type->is_lvalue() || n->get_kid(1)->get_value_type() == ValueType::COMPUTED){
        SemanticError::raise(n->get_loc(), "Cannot assign to non-lvalue");
      }
      //cannot assign to const
      else if(l_type->is_const()){
        SemanticError::raise(n->get_loc(), "Cannot assign (%s) to (const)", r_type->as_str().c_str());
      }
      //for pointer
      else if(l_type->is_pointer() && r_type->is_pointer()){
        if(!comp_ptr(l_type, r_type)){
          SemanticError::raise(n->get_loc(), "Cannot assign (%s) to (%s)", r_type->as_str().c_str(), l_type->as_str().c_str());
        }
      } else{
        if(!is_convertible(l_type, r_type)){
          SemanticError::raise(n->get_loc(), "Cannot assign (%s) to (%s)", r_type->as_str().c_str(), l_type->as_str().c_str());
        }
      }
      break;
    }
    case TOK_DIVIDE:
    case TOK_ASTERISK:
    case TOK_MOD:{
      if(l_type->is_pointer() || r_type->is_pointer()){
        SemanticError::raise(n->get_loc(), "Pointer arithmatic not allowed visit_binary_expression");
      }
    }
    case TOK_LOGICAL_AND:
    case TOK_LOGICAL_OR:
    case TOK_LT:
    case TOK_GT:
    case TOK_LTE:
    case TOK_GTE:
    case TOK_EQUALITY:
    case TOK_INEQUALITY:
    case TOK_MINUS:
    case TOK_PLUS:{
      if(l_type->is_integral() && r_type->is_integral()){
        // n->set_type(l_type);
      } else if(l_type->is_integral() && r_type->is_pointer()){
        // n->set_type(r_type);
      } else if(r_type->is_integral() && l_type->is_pointer()){
      } else{
        SemanticError::raise(n->get_loc(), "Cannot perform such binary operation");
      }
      break;
    }
    default:{
      break;
    }
  }
  n->set_value_type(ValueType::COMPUTED);
  n->set_type(l_type);
}

void SemanticAnalysis::visit_unary_expression(Node* n){
  if(debug){ puts("visit_unary_expression"); }
  visit(n->get_kid(1));
  switch(n->get_kid(0)->get_tag()){
    case TOK_ASTERISK:{
      if(!n->get_kid(1)->get_type()->is_pointer()){
        SemanticError::raise(n->get_loc(), "Cannot dereference a non-pointer");
      }
      //set type as dereferenced
      n->set_type(n->get_kid(1)->get_type()->get_base_type());
      n->set_str(n->get_kid(1)->get_str());
      break;
    }
    case TOK_AMPERSAND:{
      // !m_cur_symtab->lookup_recursive(n->get_kid(1)->get_str())
      if(!n->get_kid(1)->get_type()->is_lvalue()){
        SemanticError::raise(n->get_loc(), "Cannot get address of non-lvalue");
      }
      //set type as pointer
      std::shared_ptr<Type> pointer(new PointerType(n->get_kid(1)->get_type()));
      n->set_type(pointer);
      n->set_str(n->get_kid(1)->get_str());
      break;
    }
    default:{
      if(n->get_kid(1)->get_type()->is_pointer()){
        SemanticError::raise(n->get_loc(), "Cannot perform operation on pointer");
      }
      n->set_type(n->get_kid(1)->get_type());
      break;
    }
  }
}

void SemanticAnalysis::visit_postfix_expression(Node* n){
  if(debug){ puts("visit_postfix_expression"); }
}

void SemanticAnalysis::visit_conditional_expression(Node* n){
  if(debug){ puts("visit_conditional_expression"); }
}

void SemanticAnalysis::visit_cast_expression(Node* n){
  if(debug){ puts("visit_cast_expression"); }
}

void SemanticAnalysis::visit_function_call_expression(Node* n){
  if(debug){ puts("visit_function_call_expression"); }
  //func_name
  std::string func_name = n->get_kid(0)->get_kid(0)->get_str();
  //arg_list
  Node* arg_list_node = n->get_kid(1);
  //# of args
  int arg_cnt = arg_list_node->get_num_kids();
  Symbol* sym = m_cur_symtab->lookup_recursive_kind(func_name, SymbolKind::FUNCTION);
  if(sym == nullptr){
    SemanticError::raise(n->get_loc(), "Function %s not declared/defined", func_name.c_str());
  }
  std::shared_ptr<Type> func_type = sym->get_type();
  if(func_type->get_num_members() != arg_cnt){
    SemanticError::raise(n->get_loc(), "Function %s number of arguments does not match", func_name.c_str());
  }
  for(int i = 0; i < arg_cnt; i++){
    Node* param = arg_list_node->get_kid(i);
    visit(param);

    if(!is_convertible(param->get_type(), func_type->get_member(i).get_type())){
      SemanticError::raise(param->get_loc(), "Argument type does not match");
    }
  }
  n->set_type(func_type->get_base_type());
  n->set_str(func_name);
}

void SemanticAnalysis::visit_field_ref_expression(Node* n){
  if(debug){ puts("visit_field_ref_expression"); }
  visit(n->get_kid(0));
  std::string l_name = n->get_kid(0)->get_str();
  // if(!m_cur_symtab->lookup_recursive_kind(l_name, SymbolKind::VARIABLE)){
  //   SemanticError::raise(n->get_loc(), "%s not declared visit_field_ref_expression", l_name.c_str());
  // }
  std::shared_ptr<Type> l_type = n->get_kid(0)->get_type();
  // printf("type : %s\n", l_type->as_str().c_str());
  if(!(l_type->is_struct())){
    SemanticError::raise(n->get_loc(), "Cannot use . on non-struct");
  }
  std::string r_name = n->get_kid(1)->get_str();
  for(int i = 0; i < l_type->get_num_members(); i++){
    Member m = l_type->get_member(i);
    std::string m_name = m.get_name();
    // printf("member : %s\n", m_name.c_str());
    if(r_name == m_name){
      // printf("is l? %d\n", m.get_type()->is_lvalue());
      n->set_type(m.get_type());
      return;
    }
  }
  SemanticError::raise(n->get_loc(), "%s not declared visit_field_ref_expression", r_name.c_str());
}

void SemanticAnalysis::visit_indirect_field_ref_expression(Node* n){
  if(debug){ puts("visit_indirect_field_ref_expression"); }
  //Initial checking
  visit(n->get_kid(0));
  std::string l_name = n->get_kid(0)->get_str();
  // if(!m_cur_symtab->lookup_recursive_kind(l_name, SymbolKind::VARIABLE)){
  //   SemanticError::raise(n->get_loc(), "%s not declared visit_indirect_field_ref_expression", l_name.c_str());
  // }

  std::shared_ptr<Type> l_type = n->get_kid(0)->get_type();
  std::string r_name = n->get_kid(1)->get_str();
  //Make sure lvalue is pointer to struct
  if(!(l_type->is_pointer() && l_type->get_base_type()->is_struct())){
    SemanticError::raise(n->get_loc(), "Cannot use -> on non-(struct pointer)");
  }
  //Check if struct has field
  //first dereference the pointer to struct
  l_type = l_type->get_base_type();
  for(int i = 0; i < l_type->get_num_members(); i++){
    Member m = l_type->get_member(i);
    std::string m_name = m.get_name();
    if(r_name == m_name){
      n->set_type(m.get_type());
      return;
    }
  }
  SemanticError::raise(n->get_loc(), "%s not declared", l_name.c_str());
}

void SemanticAnalysis::visit_array_element_ref_expression(Node* n){
  if(debug){ puts("visit_array_element_ref_expression"); }
  visit(n->get_kid(0));
  visit(n->get_kid(1));
  //make sure the array index is numeric
  if(!n->get_kid(1)->get_type()->is_integral()){
    SemanticError::raise(n->get_loc(), "Cannot access non-integral index");
  }
  n->set_str(n->get_kid(0)->get_str());
  //equivalent to *(a+i), get_base_type is similar to deferencing
  // printf("field type: %d\n", n->get_kid(0)->get_type()->get_base_type()->is_lvalue());
  n->set_type(n->get_kid(0)->get_type()->get_base_type());
}

void SemanticAnalysis::visit_variable_ref(Node* n){
  if(debug){ puts("visit_variable_ref"); }
  std::string name = n->get_kid(0)->get_str();
  Symbol* v_symbol = m_cur_symtab->lookup_recursive(name);
  if(!v_symbol){
    SemanticError::raise(n->get_loc(), "%s not declared", name.c_str());
  }
  n->set_symbol(v_symbol);
  n->set_type(v_symbol->get_type());
  n->set_str(n->get_kid(0)->get_str());
}

void SemanticAnalysis::visit_literal_value(Node* n){
  if(debug){ puts("visit_literal_value"); }
  int tag = n->get_kid(0)->get_tag();
  LiteralValue result;
  std::string name = n->get_kid(0)->get_str();
  std::shared_ptr<Type> lit_type;
  switch(tag){
    case TOK_STR_LIT:{
      result = result.from_str_literal(name, n->get_loc());
      std::shared_ptr<Type> v_char(new BasicType(result));
      std::shared_ptr<Type> v_const(new QualifiedType(v_char, TypeQualifier::CONST));
      std::shared_ptr<Type> v_ptr(new PointerType(v_const));
      lit_type = v_ptr;
      break;
    }
    case TOK_INT_LIT:{
      result = result.from_int_literal(name, n->get_loc());
      std::shared_ptr<Type> v_int(new BasicType(result));
      lit_type = v_int;
      break;
    }
    case TOK_CHAR_LIT:{
      result = result.from_char_literal(name, n->get_loc());
      std::shared_ptr<Type> v_char(new BasicType(result));
      lit_type = v_char;
      break;
    }
  }
  n->set_type(lit_type);
  // printf("%s : %s\n", name.c_str(), lit_type->as_str().c_str());
  n->set_str(name);
}

void SemanticAnalysis::visit_return_expression_statement(Node* n){
  if(debug){ puts("visit_return_expression_statement"); }
  visit(n->get_kid(0));
  if(!is_convertible(n->get_kid(0)->get_type(), m_cur_symtab->get_fn_type()->get_base_type())){
    SemanticError::raise(n->get_loc(), "Does not match function return type");
  }
  n->set_type(m_cur_symtab->get_fn_type()->get_base_type());
}

void SemanticAnalysis::visit_while_statement(Node* n){
  if(debug){ puts("visit_while_statement"); }
  visit(n->get_kid(0));
  enter_scope();
  visit(n->get_kid(1));
  leave_scope();
}

void SemanticAnalysis::visit_do_while_statement(Node* n){
  if(debug){ puts("visit_do_while_statement"); }
  enter_scope();
  visit(n->get_kid(0));
  leave_scope();
  visit(n->get_kid(1));
}

void SemanticAnalysis::visit_if_statement(Node* n){
  if(debug){ puts("visit_if_statement"); }
  visit(n->get_kid(0));
  enter_scope();
  visit(n->get_kid(1));
  leave_scope();
}

void SemanticAnalysis::visit_for_statement(Node* n){
  if(debug){ puts("visit_for_statement"); }
  enter_scope();
  visit(n->get_kid(0));
  visit(n->get_kid(1));
  visit(n->get_kid(2));
  leave_scope();
  enter_scope();
  visit(n->get_kid(3));
  leave_scope();
}

void SemanticAnalysis::visit_if_else_statement(Node* n){
  if(debug){ puts("visit_if_else_statement"); }
  visit(n->get_kid(0));
  enter_scope();
  visit(n->get_kid(1));
  leave_scope();
  enter_scope();
  visit(n->get_kid(2));
  leave_scope();

}

void SemanticAnalysis::enter_scope(){
  SymbolTable* scope = new SymbolTable(m_cur_symtab);
  m_cur_symtab = scope;
}

void SemanticAnalysis::leave_scope(){
  // SymbolTable *table = m_cur_symtab;
  m_cur_symtab = m_cur_symtab->get_parent();
  // delete(table);
  assert(m_cur_symtab != nullptr);
}

// recursively build type and bring child name to the nearest node
std::string SemanticAnalysis::build_type(Node* n, std::shared_ptr<Type>& base_type){
  while(1){
    int tag = n->get_tag();
    if(tag == AST_NAMED_DECLARATOR){
      return n->get_kid(0)->get_str();
    } else if(tag == AST_ARRAY_DECLARATOR){
      int size = stoi(n->get_kid(1)->get_str());
      std::shared_ptr<Type> base_added(new ArrayType(base_type, size));
      base_type = base_added;
    } else if(tag == AST_POINTER_DECLARATOR){
      std::shared_ptr<Type> base_added(new PointerType(base_type));
      base_type = base_added;
    }
    n = n->get_kid(0);
  }
}

bool SemanticAnalysis::comp_ptr(std::shared_ptr<Type> l, std::shared_ptr<Type> r){
  while(l->has_base() && r->has_base()){
    if((l->is_pointer() && !r->is_pointer()) || (!l->is_pointer() && r->is_pointer())){
      return false;
    }
    if(!l->is_const() && r->is_const()){
      return false;
    }
    if(!l->is_volatile() && r->is_volatile()){
      return false;
    }
    l = l->get_base_type();
    r = r->get_base_type();
  }
  if(!l->is_const() && r->is_const()){
    return false;
  }
  if(!l->is_volatile() && r->is_volatile()){
    return false;
  }
  return true;
}

bool SemanticAnalysis::is_convertible(std::shared_ptr<Type> l, std::shared_ptr<Type> r){
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

Node* SemanticAnalysis::promote_to_int(Node* n){
  assert(n->get_type()->is_integral());
  assert(n->get_type()->get_basic_type_kind() < BasicTypeKind::INT);
  std::shared_ptr<Type> type(new BasicType(BasicTypeKind::INT, n->get_type()->is_signed()));
  return implicit_conversion(n, type);
}

Node* SemanticAnalysis::implicit_conversion(Node* n, const std::shared_ptr<Type>& type){
  std::unique_ptr<Node> conversion(new Node(AST_IMPLICIT_CONVERSION, { n }));
  conversion->set_type(type);
  return conversion.release();
}
