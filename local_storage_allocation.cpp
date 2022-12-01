#include <cassert>
#include "node.h"
#include "symtab.h"
#include "local_storage_allocation.h"

LocalStorageAllocation::LocalStorageAllocation()
  : m_total_local_storage(0U)
  , m_next_vreg(VREG_FIRST_LOCAL){
}

LocalStorageAllocation::~LocalStorageAllocation(){
}

bool debugg = 0;
void LocalStorageAllocation::visit_declarator_list(Node* n){
  printf("%s", debugg ? "lsa visit_declarator_list\n" : "");
  for(auto i = 0; i < n->get_num_kids(); i++){
    Node* kid = n->get_kid(i);
    std::shared_ptr<Type> var_type = kid->get_type();
    // printf("kid: %s\nSymbol: %s\n", kid->get_str().c_str(), kid->get_symbol()->as_str().c_str());
    if(var_type->is_array()){
      // printf("storage size 1 %d\n", m_total_local_storage);

      kid->get_symbol()->set_addr(m_total_local_storage);
      int siz = var_type->get_array_size();
      // printf("base size %d\n", var_type->get_base_type()->get_storage_size());
      // printf("base size %d\n", siz);
      m_malloc(siz, var_type->get_base_type()->get_storage_size());
      // printf("storage size %d\n", m_total_local_storage);
    } else if(var_type->is_struct()){
      kid->get_symbol()->set_addr(m_total_local_storage);
      int siz = var_type->get_storage_size();
      m_malloc(siz, 1);
    } else{
      m_sym.push_back(kid->get_symbol());
    }
  }
}

void LocalStorageAllocation::visit_function_definition(Node* n){
  printf("%s", debugg ? "lsa visit_function_definition\n" : "");
  m_total_local_storage = 0U;
  visit(n->get_kid(2));
  visit(n->get_kid(3));
  n->get_symbol()->set_addr(m_total_local_storage);
  // set register for every symbol
  for(auto i : m_sym){
    if(i->get_addr() == -1 && !i->get_vreg() && !i->get_from_struct()){
      i->set_vreg(m_next_vreg++);
    }
  }
  n->set_vreg(m_next_vreg);
  m_next_vreg = 10;
}

void LocalStorageAllocation::visit_function_parameter(Node* n){
  printf("%s", debugg ? "lsa visit_function_parameter\n" : "");
  m_sym.push_back(n->get_symbol());
}

void LocalStorageAllocation::visit_statement_list(Node* n){
  printf("%s", debugg ? "lsa visit_statement_list\n" : "");
  visit_children(n);
}

void LocalStorageAllocation::visit_unary_expression(Node* n){
  printf("%s", debugg ? "lsa visit_unary_expression\n" : "");
  if(n->get_kid(0)->get_tag() == TOK_AMPERSAND){
    std::shared_ptr<Type> base_type = n->get_kid(1)->get_type();
    // make sure only allocated once
    if(n->get_kid(1)->get_symbol()->get_addr() == -1){
      unsigned siz = base_type->get_storage_size();
      m_malloc(siz, 1);
      n->get_kid(1)->get_symbol()->set_addr(m_total_local_storage - siz);
    }
  }
}

//do nothing function hahahahahahaha
void LocalStorageAllocation::visit_function_declaration(Node* n){}

//do nothing function AGAIN hahahahahahaha
void LocalStorageAllocation::visit_struct_type_definition(Node* n){
  printf("%s", debugg ? "lsa visit_struct_type_def\n" : "");
  visit_children(n);
}

void LocalStorageAllocation::visit_literal_value(Node* n){
  printf("%s", debugg ? "lsa visit_literal_value\n" : "");
  // printf("name: %s\n", n->get_str().c_str());
  // printf("kind: %s\n", t->get_lit().get_str_value().c_str());
  // printf("name: %s\n", t->as_str().c_str());
  std::shared_ptr<Type> t = n->get_type();

  if(t->has_lit() && t->get_lit().get_kind() == LiteralValueKind::STRING){
    m_str_node.push_back(n);
    n->set_vreg(str_lb++);
    // m_strings.push_back(t->get_lit().get_str_value());
  }
}

// align and increment total storage allocation, number is useful for array
void LocalStorageAllocation::m_malloc(int siz, int number){
  // int i = 1;
  // while(siz > i){
  //   i = i << 1;
  // }
  int total = number * siz;
  m_total_local_storage += total;
  if(total % 8){
    m_total_local_storage += 8 - (total % 8);
  }
  // printf("base ssssize %d\n", (m_total_local_storage % i != 0) * (i - (m_total_local_storage % i)));
  // printf("base ssssize %d\n", i);
}

std::vector<Node*> LocalStorageAllocation::get_str_node(){
  return m_str_node;
}
