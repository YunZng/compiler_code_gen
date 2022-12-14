#ifndef GRAMMAR_SYMBOLS_H
#define GRAMMAR_SYMBOLS_H

#include "treeprint.h"

enum GrammarSymbol {
  NODE_TOK_LPAREN = 258,
  NODE_TOK_RPAREN,
  NODE_TOK_LBRACKET,
  NODE_TOK_RBRACKET,
  NODE_TOK_LBRACE,
  NODE_TOK_RBRACE,
  NODE_TOK_SEMICOLON,
  NODE_TOK_COLON,
  NODE_TOK_COMMA,
  NODE_TOK_DOT,
  NODE_TOK_QUESTION,
  NODE_TOK_NOT,
  NODE_TOK_ARROW,
  NODE_TOK_PLUS,
  NODE_TOK_INCREMENT,
  NODE_TOK_MINUS,
  NODE_TOK_DECREMENT,
  NODE_TOK_ASTERISK,
  NODE_TOK_DIVIDE,
  NODE_TOK_MOD,
  NODE_TOK_AMPERSAND,
  NODE_TOK_BITWISE_OR,
  NODE_TOK_BITWISE_XOR,
  NODE_TOK_BITWISE_COMPL,
  NODE_TOK_LEFT_SHIFT,
  NODE_TOK_RIGHT_SHIFT,
  NODE_TOK_LOGICAL_AND,
  NODE_TOK_LOGICAL_OR,
  NODE_TOK_EQUALITY,
  NODE_TOK_INEQUALITY,
  NODE_TOK_LT,
  NODE_TOK_LTE,
  NODE_TOK_GT,
  NODE_TOK_GTE,
  NODE_TOK_ASSIGN,
  NODE_TOK_MUL_ASSIGN,
  NODE_TOK_DIV_ASSIGN,
  NODE_TOK_MOD_ASSIGN,
  NODE_TOK_ADD_ASSIGN,
  NODE_TOK_SUB_ASSIGN,
  NODE_TOK_LEFT_ASSIGN,
  NODE_TOK_RIGHT_ASSIGN,
  NODE_TOK_AND_ASSIGN,
  NODE_TOK_XOR_ASSIGN,
  NODE_TOK_OR_ASSIGN,
  NODE_TOK_IF,
  NODE_TOK_ELSE,
  NODE_TOK_WHILE,
  NODE_TOK_FOR,
  NODE_TOK_DO,
  NODE_TOK_SWITCH,
  NODE_TOK_CASE,
  NODE_TOK_CHAR,
  NODE_TOK_SHORT,
  NODE_TOK_INT,
  NODE_TOK_LONG,
  NODE_TOK_UNSIGNED,
  NODE_TOK_SIGNED,
  NODE_TOK_FLOAT,
  NODE_TOK_DOUBLE,
  NODE_TOK_VOID,
  NODE_TOK_RETURN,
  NODE_TOK_BREAK,
  NODE_TOK_CONTINUE,
  NODE_TOK_CONST,
  NODE_TOK_VOLATILE,
  NODE_TOK_STRUCT,
  NODE_TOK_UNION,
  NODE_TOK_UNSPECIFIED_STORAGE,
  NODE_TOK_STATIC,
  NODE_TOK_EXTERN,
  NODE_TOK_AUTO,
  NODE_TOK_IDENT,
  NODE_TOK_STR_LIT,
  NODE_TOK_CHAR_LIT,
  NODE_TOK_INT_LIT,
  NODE_TOK_FP_LIT,
  NODE_unit = 1000,
  NODE_top_level_declaration,
  NODE_function_or_variable_declaration_or_definition,
  NODE_simple_variable_declaration,
  NODE_declarator_list,
  NODE_declarator,
  NODE_non_pointer_declarator,
  NODE_function_definition_or_declaration,
  NODE_function_parameter_list,
  NODE_opt_parameter_list,
  NODE_parameter_list,
  NODE_parameter,
  NODE_type,
  NODE_basic_type,
  NODE_basic_type_keyword,
  NODE_opt_statement_list,
  NODE_statement_list,
  NODE_statement,
  NODE_struct_type_definition,
  NODE_union_type_definition,
  NODE_opt_simple_variable_declaration_list,
  NODE_simple_variable_declaration_list,
  NODE_assignment_expression,
  NODE_assignment_op,
  NODE_conditional_expression,
  NODE_logical_or_expression,
  NODE_logical_and_expression,
  NODE_bitwise_or_expression,
  NODE_bitwise_xor_expression,
  NODE_bitwise_and_expression,
  NODE_equality_expression,
  NODE_relational_expression,
  NODE_relational_op,
  NODE_shift_expression,
  NODE_additive_expression,
  NODE_multiplicative_expression,
  NODE_cast_expression,
  NODE_unary_expression,
  NODE_postfix_expression,
  NODE_argument_expression_list,
  NODE_primary_expression,
};

// Get grammar symbol name corresponding to tag (enumeration value).
// Useful for making sense of a parse tree based on the tag values
// of the nodes.
const char *get_grammar_symbol_name(int tag);

// TreePrint subclass for parse trees
class ParseTreePrint : public TreePrint {
public:
  ParseTreePrint();
  ~ParseTreePrint();

  virtual std::string node_tag_to_string(int tag) const;
};

#endif // GRAMMAR_SYMBOLS_H
