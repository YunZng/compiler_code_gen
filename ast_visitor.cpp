#include "node.h"
#include "exceptions.h"
#include "ast.h"
#include "ast_visitor.h"

ASTVisitor::ASTVisitor() {
}

ASTVisitor::~ASTVisitor() {
}

void ASTVisitor::visit_unit(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_variable_declaration(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_struct_type(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_union_type(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_basic_type(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_declarator_list(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_named_declarator(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_pointer_declarator(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_array_declarator(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_function_definition(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_function_declaration(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_function_parameter_list(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_function_parameter(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_statement_list(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_empty_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_expression_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_return_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_return_expression_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_while_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_do_while_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_for_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_if_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_if_else_statement(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_struct_type_definition(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_union_type_definition(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_field_definition_list(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_binary_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_unary_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_postfix_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_conditional_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_cast_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_function_call_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_field_ref_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_indirect_field_ref_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_array_element_ref_expression(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_argument_expression_list(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_variable_ref(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_literal_value(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit_implicit_conversion(Node *n) {
  visit_children(n);
}

void ASTVisitor::visit(Node *n) {
  // assume that any node with a tag value less than 1000
  // is a token
  if (n->get_tag() < 1000) {
    visit_token(n);
    return;
  }

  switch (n->get_tag()) {
  case AST_UNIT:
    visit_unit(n); break;
  case AST_VARIABLE_DECLARATION:
    visit_variable_declaration(n); break;
  case AST_STRUCT_TYPE:
    visit_struct_type(n); break;
  case AST_UNION_TYPE:
    visit_union_type(n); break;
  case AST_BASIC_TYPE:
    visit_basic_type(n); break;
  case AST_DECLARATOR_LIST:
    visit_declarator_list(n); break;
  case AST_NAMED_DECLARATOR:
    visit_named_declarator(n); break;
  case AST_POINTER_DECLARATOR:
    visit_pointer_declarator(n); break;
  case AST_ARRAY_DECLARATOR:
    visit_array_declarator(n); break;
  case AST_FUNCTION_DEFINITION:
    visit_function_definition(n); break;
  case AST_FUNCTION_DECLARATION:
    visit_function_declaration(n); break;
  case AST_FUNCTION_PARAMETER_LIST:
    visit_function_parameter_list(n); break;
  case AST_FUNCTION_PARAMETER:
    visit_function_parameter(n); break;
  case AST_STATEMENT_LIST:
    visit_statement_list(n); break;
  case AST_EMPTY_STATEMENT:
    visit_empty_statement(n); break;
  case AST_EXPRESSION_STATEMENT:
    visit_expression_statement(n); break;
  case AST_RETURN_STATEMENT:
    visit_return_statement(n); break;
  case AST_RETURN_EXPRESSION_STATEMENT:
    visit_return_expression_statement(n); break;
  case AST_WHILE_STATEMENT:
    visit_while_statement(n); break;
  case AST_DO_WHILE_STATEMENT:
    visit_do_while_statement(n); break;
  case AST_FOR_STATEMENT:
    visit_for_statement(n); break;
  case AST_IF_STATEMENT:
    visit_if_statement(n); break;
  case AST_IF_ELSE_STATEMENT:
    visit_if_else_statement(n); break;
  case AST_STRUCT_TYPE_DEFINITION:
    visit_struct_type_definition(n); break;
  case AST_UNION_TYPE_DEFINITION:
    visit_union_type_definition(n); break;
  case AST_FIELD_DEFINITION_LIST:
    visit_field_definition_list(n); break;
  case AST_BINARY_EXPRESSION:
    visit_binary_expression(n); break;
  case AST_UNARY_EXPRESSION:
    visit_unary_expression(n); break;
  case AST_POSTFIX_EXPRESSION:
    visit_postfix_expression(n); break;
  case AST_CONDITIONAL_EXPRESSION:
    visit_conditional_expression(n); break;
  case AST_CAST_EXPRESSION:
    visit_cast_expression(n); break;
  case AST_FUNCTION_CALL_EXPRESSION:
    visit_function_call_expression(n); break;
  case AST_FIELD_REF_EXPRESSION:
    visit_field_ref_expression(n); break;
  case AST_INDIRECT_FIELD_REF_EXPRESSION:
    visit_indirect_field_ref_expression(n); break;
  case AST_ARRAY_ELEMENT_REF_EXPRESSION:
    visit_array_element_ref_expression(n); break;
  case AST_ARGUMENT_EXPRESSION_LIST:
    visit_argument_expression_list(n); break;
  case AST_VARIABLE_REF:
    visit_variable_ref(n); break;
  case AST_LITERAL_VALUE:
    visit_literal_value(n); break;
  case AST_IMPLICIT_CONVERSION:
    visit_implicit_conversion(n); break;
  default:
    RuntimeError::raise("Unknown AST node tag %d", n->get_tag());
  }
}

void ASTVisitor::visit_children(Node *n) {
  for (auto i = n->cbegin(); i != n->cend(); ++i) {
    visit(*i);
  }
}

void ASTVisitor::visit_token(Node *n) {
  // default implementation does nothing
}
