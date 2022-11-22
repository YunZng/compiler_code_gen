/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSE_TAB_H_INCLUDED
# define YY_YY_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_LPAREN = 258,              /* TOK_LPAREN  */
    TOK_RPAREN = 259,              /* TOK_RPAREN  */
    TOK_LBRACKET = 260,            /* TOK_LBRACKET  */
    TOK_RBRACKET = 261,            /* TOK_RBRACKET  */
    TOK_LBRACE = 262,              /* TOK_LBRACE  */
    TOK_RBRACE = 263,              /* TOK_RBRACE  */
    TOK_SEMICOLON = 264,           /* TOK_SEMICOLON  */
    TOK_COLON = 265,               /* TOK_COLON  */
    TOK_COMMA = 266,               /* TOK_COMMA  */
    TOK_DOT = 267,                 /* TOK_DOT  */
    TOK_QUESTION = 268,            /* TOK_QUESTION  */
    TOK_NOT = 269,                 /* TOK_NOT  */
    TOK_ARROW = 270,               /* TOK_ARROW  */
    TOK_PLUS = 271,                /* TOK_PLUS  */
    TOK_INCREMENT = 272,           /* TOK_INCREMENT  */
    TOK_MINUS = 273,               /* TOK_MINUS  */
    TOK_DECREMENT = 274,           /* TOK_DECREMENT  */
    TOK_ASTERISK = 275,            /* TOK_ASTERISK  */
    TOK_DIVIDE = 276,              /* TOK_DIVIDE  */
    TOK_MOD = 277,                 /* TOK_MOD  */
    TOK_AMPERSAND = 278,           /* TOK_AMPERSAND  */
    TOK_BITWISE_OR = 279,          /* TOK_BITWISE_OR  */
    TOK_BITWISE_XOR = 280,         /* TOK_BITWISE_XOR  */
    TOK_BITWISE_COMPL = 281,       /* TOK_BITWISE_COMPL  */
    TOK_LEFT_SHIFT = 282,          /* TOK_LEFT_SHIFT  */
    TOK_RIGHT_SHIFT = 283,         /* TOK_RIGHT_SHIFT  */
    TOK_LOGICAL_AND = 284,         /* TOK_LOGICAL_AND  */
    TOK_LOGICAL_OR = 285,          /* TOK_LOGICAL_OR  */
    TOK_EQUALITY = 286,            /* TOK_EQUALITY  */
    TOK_INEQUALITY = 287,          /* TOK_INEQUALITY  */
    TOK_LT = 288,                  /* TOK_LT  */
    TOK_LTE = 289,                 /* TOK_LTE  */
    TOK_GT = 290,                  /* TOK_GT  */
    TOK_GTE = 291,                 /* TOK_GTE  */
    TOK_ASSIGN = 292,              /* TOK_ASSIGN  */
    TOK_MUL_ASSIGN = 293,          /* TOK_MUL_ASSIGN  */
    TOK_DIV_ASSIGN = 294,          /* TOK_DIV_ASSIGN  */
    TOK_MOD_ASSIGN = 295,          /* TOK_MOD_ASSIGN  */
    TOK_ADD_ASSIGN = 296,          /* TOK_ADD_ASSIGN  */
    TOK_SUB_ASSIGN = 297,          /* TOK_SUB_ASSIGN  */
    TOK_LEFT_ASSIGN = 298,         /* TOK_LEFT_ASSIGN  */
    TOK_RIGHT_ASSIGN = 299,        /* TOK_RIGHT_ASSIGN  */
    TOK_AND_ASSIGN = 300,          /* TOK_AND_ASSIGN  */
    TOK_XOR_ASSIGN = 301,          /* TOK_XOR_ASSIGN  */
    TOK_OR_ASSIGN = 302,           /* TOK_OR_ASSIGN  */
    TOK_IF = 303,                  /* TOK_IF  */
    TOK_ELSE = 304,                /* TOK_ELSE  */
    TOK_WHILE = 305,               /* TOK_WHILE  */
    TOK_FOR = 306,                 /* TOK_FOR  */
    TOK_DO = 307,                  /* TOK_DO  */
    TOK_SWITCH = 308,              /* TOK_SWITCH  */
    TOK_CASE = 309,                /* TOK_CASE  */
    TOK_CHAR = 310,                /* TOK_CHAR  */
    TOK_SHORT = 311,               /* TOK_SHORT  */
    TOK_INT = 312,                 /* TOK_INT  */
    TOK_LONG = 313,                /* TOK_LONG  */
    TOK_UNSIGNED = 314,            /* TOK_UNSIGNED  */
    TOK_SIGNED = 315,              /* TOK_SIGNED  */
    TOK_FLOAT = 316,               /* TOK_FLOAT  */
    TOK_DOUBLE = 317,              /* TOK_DOUBLE  */
    TOK_VOID = 318,                /* TOK_VOID  */
    TOK_RETURN = 319,              /* TOK_RETURN  */
    TOK_BREAK = 320,               /* TOK_BREAK  */
    TOK_CONTINUE = 321,            /* TOK_CONTINUE  */
    TOK_CONST = 322,               /* TOK_CONST  */
    TOK_VOLATILE = 323,            /* TOK_VOLATILE  */
    TOK_STRUCT = 324,              /* TOK_STRUCT  */
    TOK_UNION = 325,               /* TOK_UNION  */
    TOK_UNSPECIFIED_STORAGE = 326, /* TOK_UNSPECIFIED_STORAGE  */
    TOK_STATIC = 327,              /* TOK_STATIC  */
    TOK_EXTERN = 328,              /* TOK_EXTERN  */
    TOK_AUTO = 329,                /* TOK_AUTO  */
    TOK_IDENT = 330,               /* TOK_IDENT  */
    TOK_STR_LIT = 331,             /* TOK_STR_LIT  */
    TOK_CHAR_LIT = 332,            /* TOK_CHAR_LIT  */
    TOK_INT_LIT = 333,             /* TOK_INT_LIT  */
    TOK_FP_LIT = 334               /* TOK_FP_LIT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 77 "parse_buildast.y"

  Node *node;

#line 147 "parse.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif




int yyparse (struct ParserState *pp);


#endif /* !YY_YY_PARSE_TAB_H_INCLUDED  */
