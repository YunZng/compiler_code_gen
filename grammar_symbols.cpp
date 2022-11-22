#include "grammar_symbols.h"

namespace {
const char *s_grammar_symbol_names[] = {
  "TOK_LPAREN",
  "TOK_RPAREN",
  "TOK_LBRACKET",
  "TOK_RBRACKET",
  "TOK_LBRACE",
  "TOK_RBRACE",
  "TOK_SEMICOLON",
  "TOK_COLON",
  "TOK_COMMA",
  "TOK_DOT",
  "TOK_QUESTION",
  "TOK_NOT",
  "TOK_ARROW",
  "TOK_PLUS",
  "TOK_INCREMENT",
  "TOK_MINUS",
  "TOK_DECREMENT",
  "TOK_ASTERISK",
  "TOK_DIVIDE",
  "TOK_MOD",
  "TOK_AMPERSAND",
  "TOK_BITWISE_OR",
  "TOK_BITWISE_XOR",
  "TOK_BITWISE_COMPL",
  "TOK_LEFT_SHIFT",
  "TOK_RIGHT_SHIFT",
  "TOK_LOGICAL_AND",
  "TOK_LOGICAL_OR",
  "TOK_EQUALITY",
  "TOK_INEQUALITY",
  "TOK_LT",
  "TOK_LTE",
  "TOK_GT",
  "TOK_GTE",
  "TOK_ASSIGN",
  "TOK_MUL_ASSIGN",
  "TOK_DIV_ASSIGN",
  "TOK_MOD_ASSIGN",
  "TOK_ADD_ASSIGN",
  "TOK_SUB_ASSIGN",
  "TOK_LEFT_ASSIGN",
  "TOK_RIGHT_ASSIGN",
  "TOK_AND_ASSIGN",
  "TOK_XOR_ASSIGN",
  "TOK_OR_ASSIGN",
  "TOK_IF",
  "TOK_ELSE",
  "TOK_WHILE",
  "TOK_FOR",
  "TOK_DO",
  "TOK_SWITCH",
  "TOK_CASE",
  "TOK_CHAR",
  "TOK_SHORT",
  "TOK_INT",
  "TOK_LONG",
  "TOK_UNSIGNED",
  "TOK_SIGNED",
  "TOK_FLOAT",
  "TOK_DOUBLE",
  "TOK_VOID",
  "TOK_RETURN",
  "TOK_BREAK",
  "TOK_CONTINUE",
  "TOK_CONST",
  "TOK_VOLATILE",
  "TOK_STRUCT",
  "TOK_UNION",
  "TOK_UNSPECIFIED_STORAGE",
  "TOK_STATIC",
  "TOK_EXTERN",
  "TOK_AUTO",
  "TOK_IDENT",
  "TOK_STR_LIT",
  "TOK_CHAR_LIT",
  "TOK_INT_LIT",
  "TOK_FP_LIT",
  "unit",
  "top_level_declaration",
  "function_or_variable_declaration_or_definition",
  "simple_variable_declaration",
  "declarator_list",
  "declarator",
  "non_pointer_declarator",
  "function_definition_or_declaration",
  "function_parameter_list",
  "opt_parameter_list",
  "parameter_list",
  "parameter",
  "type",
  "basic_type",
  "basic_type_keyword",
  "opt_statement_list",
  "statement_list",
  "statement",
  "struct_type_definition",
  "union_type_definition",
  "opt_simple_variable_declaration_list",
  "simple_variable_declaration_list",
  "assignment_expression",
  "assignment_op",
  "conditional_expression",
  "logical_or_expression",
  "logical_and_expression",
  "bitwise_or_expression",
  "bitwise_xor_expression",
  "bitwise_and_expression",
  "equality_expression",
  "relational_expression",
  "relational_op",
  "shift_expression",
  "additive_expression",
  "multiplicative_expression",
  "cast_expression",
  "unary_expression",
  "postfix_expression",
  "argument_expression_list",
  "primary_expression",
};
}

const char *get_grammar_symbol_name(int tag) {
  if (tag < 258) {
    return NULL;
  }

  if (tag < 1000) {
    // must be a token
    int which_token = tag - 258;
    if (which_token >= 77) {
      return NULL;
    } else {
      return s_grammar_symbol_names[which_token];
    }
  }

  int which_production = tag - 1000;
  if (which_production >= 41) {
    return NULL;
  }
  return s_grammar_symbol_names[77 + which_production];
}

ParseTreePrint::ParseTreePrint() {
}

ParseTreePrint::~ParseTreePrint() {
}

std::string ParseTreePrint::node_tag_to_string(int tag) const {
  return std::string(get_grammar_symbol_name(tag));
}
