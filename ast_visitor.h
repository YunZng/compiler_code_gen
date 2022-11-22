#ifndef AST_VISITOR_H
#define AST_VISITOR_H

class Node;

class ASTVisitor {
public:
  ASTVisitor();
  virtual ~ASTVisitor();

  virtual void visit(Node *n);
  virtual void visit_unit(Node *n);
  virtual void visit_variable_declaration(Node *n);
  virtual void visit_struct_type(Node *n);
  virtual void visit_union_type(Node *n);
  virtual void visit_basic_type(Node *n);
  virtual void visit_declarator_list(Node *n);
  virtual void visit_named_declarator(Node *n);
  virtual void visit_pointer_declarator(Node *n);
  virtual void visit_array_declarator(Node *n);
  virtual void visit_function_definition(Node *n);
  virtual void visit_function_declaration(Node *n);
  virtual void visit_function_parameter_list(Node *n);
  virtual void visit_function_parameter(Node *n);
  virtual void visit_statement_list(Node *n);
  virtual void visit_empty_statement(Node *n);
  virtual void visit_expression_statement(Node *n);
  virtual void visit_return_statement(Node *n);
  virtual void visit_return_expression_statement(Node *n);
  virtual void visit_while_statement(Node *n);
  virtual void visit_do_while_statement(Node *n);
  virtual void visit_for_statement(Node *n);
  virtual void visit_if_statement(Node *n);
  virtual void visit_if_else_statement(Node *n);
  virtual void visit_struct_type_definition(Node *n);
  virtual void visit_union_type_definition(Node *n);
  virtual void visit_field_definition_list(Node *n);
  virtual void visit_binary_expression(Node *n);
  virtual void visit_unary_expression(Node *n);
  virtual void visit_postfix_expression(Node *n);
  virtual void visit_conditional_expression(Node *n);
  virtual void visit_cast_expression(Node *n);
  virtual void visit_function_call_expression(Node *n);
  virtual void visit_field_ref_expression(Node *n);
  virtual void visit_indirect_field_ref_expression(Node *n);
  virtual void visit_array_element_ref_expression(Node *n);
  virtual void visit_argument_expression_list(Node *n);
  virtual void visit_variable_ref(Node *n);
  virtual void visit_literal_value(Node *n);
  virtual void visit_implicit_conversion(Node *n);
  virtual void visit_children(Node *n);

  virtual void visit_token(Node *n);
};

#endif // AST_VISITOR_H
