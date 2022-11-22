/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parse_buildast.y"

// Copyright (c) 2021-2022, David H. Hovemeyer <david.hovemeyer@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// parse.y
// This is the parser that builds ASTs

#include "node.h"
#include "parser_state.h"
#include "grammar_symbols.h"
#include "ast.h"
#include "yyerror.h"

// This is a weird hack required to make bison pass the
// yyscan_t value representing the scanner state (which is
// stored in the ParserState object) to yylex()
#define the_scanner pp->scan_info

// Bison does not actually declare yylex()
typedef union YYSTYPE YYSTYPE;
int yylex(YYSTYPE *, void *);

namespace {
  // All variable declarations default to having "unspecified" storage.
  // If an explicit storage class (static or extern) is specified,
  // this will be overridden.
  void handle_unspecified_storage(Node *ast, struct ParserState *pp) {
    Node *first_kid = ast->get_kid(0);
    Node *unspecified_storage = new Node(NODE_TOK_UNSPECIFIED_STORAGE);
    unspecified_storage->set_loc(first_kid->get_loc());
    ast->prepend_kid(unspecified_storage);
    pp->tokens.push_back(unspecified_storage);
  }
}

#line 124 "parse.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parse.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_LPAREN = 3,                 /* TOK_LPAREN  */
  YYSYMBOL_TOK_RPAREN = 4,                 /* TOK_RPAREN  */
  YYSYMBOL_TOK_LBRACKET = 5,               /* TOK_LBRACKET  */
  YYSYMBOL_TOK_RBRACKET = 6,               /* TOK_RBRACKET  */
  YYSYMBOL_TOK_LBRACE = 7,                 /* TOK_LBRACE  */
  YYSYMBOL_TOK_RBRACE = 8,                 /* TOK_RBRACE  */
  YYSYMBOL_TOK_SEMICOLON = 9,              /* TOK_SEMICOLON  */
  YYSYMBOL_TOK_COLON = 10,                 /* TOK_COLON  */
  YYSYMBOL_TOK_COMMA = 11,                 /* TOK_COMMA  */
  YYSYMBOL_TOK_DOT = 12,                   /* TOK_DOT  */
  YYSYMBOL_TOK_QUESTION = 13,              /* TOK_QUESTION  */
  YYSYMBOL_TOK_NOT = 14,                   /* TOK_NOT  */
  YYSYMBOL_TOK_ARROW = 15,                 /* TOK_ARROW  */
  YYSYMBOL_TOK_PLUS = 16,                  /* TOK_PLUS  */
  YYSYMBOL_TOK_INCREMENT = 17,             /* TOK_INCREMENT  */
  YYSYMBOL_TOK_MINUS = 18,                 /* TOK_MINUS  */
  YYSYMBOL_TOK_DECREMENT = 19,             /* TOK_DECREMENT  */
  YYSYMBOL_TOK_ASTERISK = 20,              /* TOK_ASTERISK  */
  YYSYMBOL_TOK_DIVIDE = 21,                /* TOK_DIVIDE  */
  YYSYMBOL_TOK_MOD = 22,                   /* TOK_MOD  */
  YYSYMBOL_TOK_AMPERSAND = 23,             /* TOK_AMPERSAND  */
  YYSYMBOL_TOK_BITWISE_OR = 24,            /* TOK_BITWISE_OR  */
  YYSYMBOL_TOK_BITWISE_XOR = 25,           /* TOK_BITWISE_XOR  */
  YYSYMBOL_TOK_BITWISE_COMPL = 26,         /* TOK_BITWISE_COMPL  */
  YYSYMBOL_TOK_LEFT_SHIFT = 27,            /* TOK_LEFT_SHIFT  */
  YYSYMBOL_TOK_RIGHT_SHIFT = 28,           /* TOK_RIGHT_SHIFT  */
  YYSYMBOL_TOK_LOGICAL_AND = 29,           /* TOK_LOGICAL_AND  */
  YYSYMBOL_TOK_LOGICAL_OR = 30,            /* TOK_LOGICAL_OR  */
  YYSYMBOL_TOK_EQUALITY = 31,              /* TOK_EQUALITY  */
  YYSYMBOL_TOK_INEQUALITY = 32,            /* TOK_INEQUALITY  */
  YYSYMBOL_TOK_LT = 33,                    /* TOK_LT  */
  YYSYMBOL_TOK_LTE = 34,                   /* TOK_LTE  */
  YYSYMBOL_TOK_GT = 35,                    /* TOK_GT  */
  YYSYMBOL_TOK_GTE = 36,                   /* TOK_GTE  */
  YYSYMBOL_TOK_ASSIGN = 37,                /* TOK_ASSIGN  */
  YYSYMBOL_TOK_MUL_ASSIGN = 38,            /* TOK_MUL_ASSIGN  */
  YYSYMBOL_TOK_DIV_ASSIGN = 39,            /* TOK_DIV_ASSIGN  */
  YYSYMBOL_TOK_MOD_ASSIGN = 40,            /* TOK_MOD_ASSIGN  */
  YYSYMBOL_TOK_ADD_ASSIGN = 41,            /* TOK_ADD_ASSIGN  */
  YYSYMBOL_TOK_SUB_ASSIGN = 42,            /* TOK_SUB_ASSIGN  */
  YYSYMBOL_TOK_LEFT_ASSIGN = 43,           /* TOK_LEFT_ASSIGN  */
  YYSYMBOL_TOK_RIGHT_ASSIGN = 44,          /* TOK_RIGHT_ASSIGN  */
  YYSYMBOL_TOK_AND_ASSIGN = 45,            /* TOK_AND_ASSIGN  */
  YYSYMBOL_TOK_XOR_ASSIGN = 46,            /* TOK_XOR_ASSIGN  */
  YYSYMBOL_TOK_OR_ASSIGN = 47,             /* TOK_OR_ASSIGN  */
  YYSYMBOL_TOK_IF = 48,                    /* TOK_IF  */
  YYSYMBOL_TOK_ELSE = 49,                  /* TOK_ELSE  */
  YYSYMBOL_TOK_WHILE = 50,                 /* TOK_WHILE  */
  YYSYMBOL_TOK_FOR = 51,                   /* TOK_FOR  */
  YYSYMBOL_TOK_DO = 52,                    /* TOK_DO  */
  YYSYMBOL_TOK_SWITCH = 53,                /* TOK_SWITCH  */
  YYSYMBOL_TOK_CASE = 54,                  /* TOK_CASE  */
  YYSYMBOL_TOK_CHAR = 55,                  /* TOK_CHAR  */
  YYSYMBOL_TOK_SHORT = 56,                 /* TOK_SHORT  */
  YYSYMBOL_TOK_INT = 57,                   /* TOK_INT  */
  YYSYMBOL_TOK_LONG = 58,                  /* TOK_LONG  */
  YYSYMBOL_TOK_UNSIGNED = 59,              /* TOK_UNSIGNED  */
  YYSYMBOL_TOK_SIGNED = 60,                /* TOK_SIGNED  */
  YYSYMBOL_TOK_FLOAT = 61,                 /* TOK_FLOAT  */
  YYSYMBOL_TOK_DOUBLE = 62,                /* TOK_DOUBLE  */
  YYSYMBOL_TOK_VOID = 63,                  /* TOK_VOID  */
  YYSYMBOL_TOK_RETURN = 64,                /* TOK_RETURN  */
  YYSYMBOL_TOK_BREAK = 65,                 /* TOK_BREAK  */
  YYSYMBOL_TOK_CONTINUE = 66,              /* TOK_CONTINUE  */
  YYSYMBOL_TOK_CONST = 67,                 /* TOK_CONST  */
  YYSYMBOL_TOK_VOLATILE = 68,              /* TOK_VOLATILE  */
  YYSYMBOL_TOK_STRUCT = 69,                /* TOK_STRUCT  */
  YYSYMBOL_TOK_UNION = 70,                 /* TOK_UNION  */
  YYSYMBOL_TOK_UNSPECIFIED_STORAGE = 71,   /* TOK_UNSPECIFIED_STORAGE  */
  YYSYMBOL_TOK_STATIC = 72,                /* TOK_STATIC  */
  YYSYMBOL_TOK_EXTERN = 73,                /* TOK_EXTERN  */
  YYSYMBOL_TOK_AUTO = 74,                  /* TOK_AUTO  */
  YYSYMBOL_TOK_IDENT = 75,                 /* TOK_IDENT  */
  YYSYMBOL_TOK_STR_LIT = 76,               /* TOK_STR_LIT  */
  YYSYMBOL_TOK_CHAR_LIT = 77,              /* TOK_CHAR_LIT  */
  YYSYMBOL_TOK_INT_LIT = 78,               /* TOK_INT_LIT  */
  YYSYMBOL_TOK_FP_LIT = 79,                /* TOK_FP_LIT  */
  YYSYMBOL_YYACCEPT = 80,                  /* $accept  */
  YYSYMBOL_unit = 81,                      /* unit  */
  YYSYMBOL_top_level_declaration = 82,     /* top_level_declaration  */
  YYSYMBOL_function_or_variable_declaration_or_definition = 83, /* function_or_variable_declaration_or_definition  */
  YYSYMBOL_simple_variable_declaration = 84, /* simple_variable_declaration  */
  YYSYMBOL_declarator_list = 85,           /* declarator_list  */
  YYSYMBOL_declarator = 86,                /* declarator  */
  YYSYMBOL_non_pointer_declarator = 87,    /* non_pointer_declarator  */
  YYSYMBOL_function_definition_or_declaration = 88, /* function_definition_or_declaration  */
  YYSYMBOL_function_parameter_list = 89,   /* function_parameter_list  */
  YYSYMBOL_opt_parameter_list = 90,        /* opt_parameter_list  */
  YYSYMBOL_parameter_list = 91,            /* parameter_list  */
  YYSYMBOL_parameter = 92,                 /* parameter  */
  YYSYMBOL_type = 93,                      /* type  */
  YYSYMBOL_basic_type = 94,                /* basic_type  */
  YYSYMBOL_basic_type_keyword = 95,        /* basic_type_keyword  */
  YYSYMBOL_opt_statement_list = 96,        /* opt_statement_list  */
  YYSYMBOL_statement_list = 97,            /* statement_list  */
  YYSYMBOL_statement = 98,                 /* statement  */
  YYSYMBOL_struct_type_definition = 99,    /* struct_type_definition  */
  YYSYMBOL_union_type_definition = 100,    /* union_type_definition  */
  YYSYMBOL_opt_simple_variable_declaration_list = 101, /* opt_simple_variable_declaration_list  */
  YYSYMBOL_simple_variable_declaration_list = 102, /* simple_variable_declaration_list  */
  YYSYMBOL_assignment_expression = 103,    /* assignment_expression  */
  YYSYMBOL_assignment_op = 104,            /* assignment_op  */
  YYSYMBOL_conditional_expression = 105,   /* conditional_expression  */
  YYSYMBOL_logical_or_expression = 106,    /* logical_or_expression  */
  YYSYMBOL_logical_and_expression = 107,   /* logical_and_expression  */
  YYSYMBOL_bitwise_or_expression = 108,    /* bitwise_or_expression  */
  YYSYMBOL_bitwise_xor_expression = 109,   /* bitwise_xor_expression  */
  YYSYMBOL_bitwise_and_expression = 110,   /* bitwise_and_expression  */
  YYSYMBOL_equality_expression = 111,      /* equality_expression  */
  YYSYMBOL_relational_expression = 112,    /* relational_expression  */
  YYSYMBOL_relational_op = 113,            /* relational_op  */
  YYSYMBOL_shift_expression = 114,         /* shift_expression  */
  YYSYMBOL_additive_expression = 115,      /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 116, /* multiplicative_expression  */
  YYSYMBOL_cast_expression = 117,          /* cast_expression  */
  YYSYMBOL_unary_expression = 118,         /* unary_expression  */
  YYSYMBOL_postfix_expression = 119,       /* postfix_expression  */
  YYSYMBOL_argument_expression_list = 120, /* argument_expression_list  */
  YYSYMBOL_primary_expression = 121        /* primary_expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   440

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  234

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   334


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   146,   146,   148,   153,   155,   157,   159,   161,   166,
     168,   173,   178,   180,   186,   188,   194,   196,   201,   203,
     208,   210,   215,   218,   222,   224,   229,   234,   236,   238,
     248,   250,   255,   257,   259,   261,   263,   265,   267,   269,
     271,   273,   275,   280,   283,   287,   289,   294,   296,   298,
     300,   302,   304,   306,   308,   310,   312,   319,   323,   325,
     330,   335,   340,   343,   347,   349,   368,   370,   375,   377,
     379,   381,   383,   385,   387,   389,   391,   393,   395,   400,
     402,   407,   409,   414,   416,   421,   423,   428,   430,   435,
     437,   442,   444,   446,   451,   453,   458,   460,   462,   464,
     469,   471,   473,   478,   480,   482,   487,   489,   491,   493,
     498,   500,   505,   507,   509,   511,   513,   515,   517,   519,
     521,   534,   536,   538,   540,   542,   544,   546,   548,   553,
     555,   560,   562,   564,   566,   568,   570
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_LPAREN",
  "TOK_RPAREN", "TOK_LBRACKET", "TOK_RBRACKET", "TOK_LBRACE", "TOK_RBRACE",
  "TOK_SEMICOLON", "TOK_COLON", "TOK_COMMA", "TOK_DOT", "TOK_QUESTION",
  "TOK_NOT", "TOK_ARROW", "TOK_PLUS", "TOK_INCREMENT", "TOK_MINUS",
  "TOK_DECREMENT", "TOK_ASTERISK", "TOK_DIVIDE", "TOK_MOD",
  "TOK_AMPERSAND", "TOK_BITWISE_OR", "TOK_BITWISE_XOR",
  "TOK_BITWISE_COMPL", "TOK_LEFT_SHIFT", "TOK_RIGHT_SHIFT",
  "TOK_LOGICAL_AND", "TOK_LOGICAL_OR", "TOK_EQUALITY", "TOK_INEQUALITY",
  "TOK_LT", "TOK_LTE", "TOK_GT", "TOK_GTE", "TOK_ASSIGN", "TOK_MUL_ASSIGN",
  "TOK_DIV_ASSIGN", "TOK_MOD_ASSIGN", "TOK_ADD_ASSIGN", "TOK_SUB_ASSIGN",
  "TOK_LEFT_ASSIGN", "TOK_RIGHT_ASSIGN", "TOK_AND_ASSIGN",
  "TOK_XOR_ASSIGN", "TOK_OR_ASSIGN", "TOK_IF", "TOK_ELSE", "TOK_WHILE",
  "TOK_FOR", "TOK_DO", "TOK_SWITCH", "TOK_CASE", "TOK_CHAR", "TOK_SHORT",
  "TOK_INT", "TOK_LONG", "TOK_UNSIGNED", "TOK_SIGNED", "TOK_FLOAT",
  "TOK_DOUBLE", "TOK_VOID", "TOK_RETURN", "TOK_BREAK", "TOK_CONTINUE",
  "TOK_CONST", "TOK_VOLATILE", "TOK_STRUCT", "TOK_UNION",
  "TOK_UNSPECIFIED_STORAGE", "TOK_STATIC", "TOK_EXTERN", "TOK_AUTO",
  "TOK_IDENT", "TOK_STR_LIT", "TOK_CHAR_LIT", "TOK_INT_LIT", "TOK_FP_LIT",
  "$accept", "unit", "top_level_declaration",
  "function_or_variable_declaration_or_definition",
  "simple_variable_declaration", "declarator_list", "declarator",
  "non_pointer_declarator", "function_definition_or_declaration",
  "function_parameter_list", "opt_parameter_list", "parameter_list",
  "parameter", "type", "basic_type", "basic_type_keyword",
  "opt_statement_list", "statement_list", "statement",
  "struct_type_definition", "union_type_definition",
  "opt_simple_variable_declaration_list",
  "simple_variable_declaration_list", "assignment_expression",
  "assignment_op", "conditional_expression", "logical_or_expression",
  "logical_and_expression", "bitwise_or_expression",
  "bitwise_xor_expression", "bitwise_and_expression",
  "equality_expression", "relational_expression", "relational_op",
  "shift_expression", "additive_expression", "multiplicative_expression",
  "cast_expression", "unary_expression", "postfix_expression",
  "argument_expression_list", "primary_expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-93)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-21)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     337,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -72,   -48,   297,   297,    35,   337,   -93,   -93,
     -93,     2,   -93,   372,   -93,   -93,    33,    37,   -33,   -29,
     -93,   -93,   -93,   -93,     3,    62,    57,    78,    63,   -93,
     297,   297,   -93,   -93,   -93,   -93,   356,   -93,     3,    12,
     297,     3,    83,   -93,    84,    89,    93,   -93,   -93,    91,
       3,   -93,    95,   -93,    96,    97,    23,   297,   -93,   -93,
     -93,   -93,   142,   -93,   -93,   210,   142,   -93,   294,   294,
     312,   294,   312,   312,   312,   294,   101,   105,   109,   142,
     100,   297,   297,   -93,   -93,   -93,   -93,   -93,   -93,   113,
     -93,   142,   104,   -93,    -9,   112,    98,   121,   124,   -14,
      17,    20,    21,     4,   -93,    90,    26,   -93,   144,   146,
     145,   -93,   -93,   -93,   294,   -93,   -93,   -93,   -93,   -93,
     -93,   294,   294,   294,   107,   -93,   154,   -93,   -93,   -93,
     -93,   -93,   294,   294,   294,   294,   294,   294,   294,   294,
     -93,   -93,   -93,   -93,   294,   294,   294,   294,   294,   294,
     294,   294,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   294,   228,   294,    79,    80,   -93,   -93,
     294,   -93,   -93,   160,   162,   158,   166,   -93,   161,   112,
      98,   121,   124,   -14,    17,    17,    20,    21,    21,     4,
       4,   -93,   -93,   -93,   -93,   -93,   159,   168,   167,   -93,
     -93,   -93,   142,   142,   294,   294,   294,   294,   -93,   -93,
     125,   -93,   171,   177,   -93,   -93,   142,   294,   173,   -93,
     179,   -93,   142,   -93
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,     2,     4,    10,
       9,     0,    27,    30,     7,     8,    28,    29,     0,     0,
       5,     6,     1,     3,     0,    16,     0,    12,    15,    31,
      63,    63,    28,    29,    16,    14,    23,    11,     0,     0,
      64,     0,     0,    62,     0,    40,     0,    21,    22,    24,
       0,    13,     0,    65,     0,     0,     0,     0,    26,    17,
      60,    61,    44,    19,    25,     0,    44,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   134,   132,   131,   133,    48,     0,
      43,    45,     0,    67,    79,    81,    83,    85,    87,    89,
      91,    94,   100,   103,   106,   110,   112,   121,     0,     0,
       0,   115,   110,   113,     0,   117,   114,   118,   119,   120,
     116,     0,     0,     0,     0,    52,     0,    49,    50,    18,
      46,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,     0,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,     0,     0,     0,     0,   122,   123,
       0,   136,    54,     0,     0,     0,     0,    53,     0,    82,
      84,    86,    88,    90,    92,    93,    95,   101,   102,   104,
     105,   107,   108,   109,    66,   124,   129,     0,     0,   126,
     127,   111,     0,     0,     0,     0,     0,     0,   125,   128,
      58,    55,     0,     0,    80,   130,     0,     0,     0,    59,
       0,    56,     0,    57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   169,   -93,    40,    19,   136,   -32,   -93,   -93,   -93,
     -93,   118,   -93,   276,   164,   -93,   115,    87,   -88,   -93,
     -93,   148,   157,   -75,   -93,   -21,   -93,    53,    64,    71,
      76,    88,   -87,   -93,    69,   -92,   -78,   -65,   -73,   -93,
       8,   -93
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    16,    17,    18,    98,    36,    37,    38,    20,    56,
      57,    58,    59,    51,    22,    23,    99,   100,   101,    24,
      25,    52,    53,   102,   173,   103,   104,   105,   106,   107,
     108,   109,   110,   154,   111,   112,   113,   114,   115,   116,
     207,   117
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     119,   134,    45,    26,   142,   122,   122,   125,   122,   127,
     128,   129,   122,   121,   123,   136,   126,   148,   149,    19,
     130,   143,    34,    34,   159,   160,   161,    27,    68,   174,
      72,   175,    73,    19,    19,    32,    19,   157,   176,   158,
      40,   177,    42,   178,    41,   179,    43,   155,   156,   119,
     150,   151,   152,   153,    30,    31,   183,   184,   185,    50,
      50,   194,   195,   197,   198,    46,    47,   188,    49,    50,
     122,   122,   122,   122,   122,   122,   122,    35,    44,   199,
     200,   122,   122,   122,   122,   122,   122,   122,   122,    48,
      62,    64,    65,   -20,   201,   202,   203,    66,   204,   206,
     208,    69,    67,    75,   131,    70,    71,   122,   132,   135,
     137,   138,   133,   141,    78,   211,    79,    80,    81,    82,
      83,   139,   145,    84,   220,   221,    85,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   229,   222,
     223,   144,   206,   122,   233,    75,   146,   147,   180,    76,
     181,    77,   230,   182,   209,   210,    78,   186,    79,    80,
      81,    82,    83,   187,   212,    84,   213,   214,    85,   215,
     217,   216,   218,   219,   226,    93,    94,    95,    96,    97,
     227,   228,   231,   232,    61,    74,    33,    39,   140,    54,
      86,   120,    87,    88,    89,   224,   189,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    90,    63,   190,    10,
      11,    28,    29,    75,    91,    92,   191,    93,    94,    95,
      96,    97,   192,   196,    78,   225,    79,    80,    81,    82,
      83,    75,   205,    84,     0,   193,    85,     0,     0,     0,
       0,     0,    78,     0,    79,    80,    81,    82,    83,     0,
       0,    84,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     2,     3,     4,     5,
       6,     7,     8,     9,     0,     0,    21,    10,    11,    28,
      29,     0,     0,     0,     0,    93,    94,    95,    96,    97,
      21,    21,     0,    21,     0,     0,     0,    75,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,    78,     0,
      79,    80,    81,    82,    83,   124,     0,    84,     0,     0,
      85,     0,    60,     0,     0,     0,    78,     0,    79,    80,
      81,    82,    83,     0,     0,    84,     0,     0,    85,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,   118,     1,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,    28,    29,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     1,     2,     3,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,    12,    13,     0,    14,
      15,     1,     2,     3,     4,     5,     6,     7,     8,    55,
       0,     0,     0,    10,    11,    28,    29,     1,     2,     3,
       4,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11
};

static const yytype_int16 yycheck[] =
{
      75,    89,    34,    75,    13,    78,    79,    80,    81,    82,
      83,    84,    85,    78,    79,    90,    81,    31,    32,     0,
      85,    30,    20,    20,    20,    21,    22,    75,    60,     3,
       7,     5,     9,    14,    15,     0,    17,    16,    12,    18,
       7,    15,    75,    17,     7,    19,    75,    27,    28,   124,
      33,    34,    35,    36,    14,    15,   131,   132,   133,    40,
      41,   148,   149,   155,   156,     3,     9,   142,     5,    50,
     143,   144,   145,   146,   147,   148,   149,    75,    75,   157,
     158,   154,   155,   156,   157,   158,   159,   160,   161,    11,
      78,     8,     8,     4,   159,   160,   161,     4,   173,   174,
     175,     6,    11,     3,     3,     9,     9,   180,     3,     9,
      91,    92,     3,     9,    14,   180,    16,    17,    18,    19,
      20,     8,    24,    23,   212,   213,    26,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   226,   214,
     215,    29,   217,   216,   232,     3,    25,    23,     4,     7,
       4,     9,   227,     8,    75,    75,    14,    50,    16,    17,
      18,    19,    20,     9,     4,    23,     4,     9,    26,     3,
      11,    10,     4,     6,    49,    75,    76,    77,    78,    79,
       9,     4,     9,     4,    48,    67,    17,    23,   101,    41,
      48,    76,    50,    51,    52,   216,   143,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    50,   144,    67,
      68,    69,    70,     3,    72,    73,   145,    75,    76,    77,
      78,    79,   146,   154,    14,   217,    16,    17,    18,    19,
      20,     3,     4,    23,    -1,   147,    26,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    16,    17,    18,    19,    20,    -1,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,     0,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,
      14,    15,    -1,    17,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    78,    79,    14,    -1,
      16,    17,    18,    19,    20,     3,    -1,    23,    -1,    -1,
      26,    -1,    46,    -1,    -1,    -1,    14,    -1,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    75,
      76,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      78,    79,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,
      73,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,    70,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    -1,    67,
      68
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      67,    68,    69,    70,    72,    73,    81,    82,    83,    84,
      88,    93,    94,    95,    99,   100,    75,    75,    69,    70,
      83,    83,     0,    81,    20,    75,    85,    86,    87,    94,
       7,     7,    75,    75,    75,    86,     3,     9,    11,     5,
      84,    93,   101,   102,   101,    63,    89,    90,    91,    92,
      93,    85,    78,   102,     8,     8,     4,    11,    86,     6,
       9,     9,     7,     9,    91,     3,     7,     9,    14,    16,
      17,    18,    19,    20,    23,    26,    48,    50,    51,    52,
      64,    72,    73,    75,    76,    77,    78,    79,    84,    96,
      97,    98,   103,   105,   106,   107,   108,   109,   110,   111,
     112,   114,   115,   116,   117,   118,   119,   121,    93,   103,
      96,   117,   118,   117,     3,   118,   117,   118,   118,   118,
     117,     3,     3,     3,    98,     9,   103,    84,    84,     8,
      97,     9,    13,    30,    29,    24,    25,    23,    31,    32,
      33,    34,    35,    36,   113,    27,    28,    16,    18,    20,
      21,    22,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,   104,     3,     5,    12,    15,    17,    19,
       4,     4,     8,   103,   103,   103,    50,     9,   103,   107,
     108,   109,   110,   111,   112,   112,   114,   115,   115,   116,
     116,   117,   117,   117,   103,     4,   103,   120,   103,    75,
      75,   117,     4,     4,     9,     3,    10,    11,     4,     6,
      98,    98,   103,   103,   105,   120,    49,     9,     4,    98,
     103,     9,     4,    98
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    80,    81,    81,    82,    82,    82,    82,    82,    83,
      83,    84,    85,    85,    86,    86,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    93,    93,    93,
      94,    94,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    96,    96,    97,    97,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      99,   100,   101,   101,   102,   102,   103,   103,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   105,
     105,   106,   106,   107,   107,   108,   108,   109,   109,   110,
     110,   111,   111,   111,   112,   112,   113,   113,   113,   113,
     114,   114,   114,   115,   115,   115,   116,   116,   116,   116,
     117,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   119,   119,   119,   119,   119,   119,   119,   120,
     120,   121,   121,   121,   121,   121,   121
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     1,     1,     1,
       1,     3,     1,     3,     2,     1,     1,     4,     8,     6,
       1,     1,     1,     0,     1,     3,     2,     1,     2,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     2,
       2,     2,     2,     3,     3,     5,     7,     9,     5,     7,
       6,     6,     1,     0,     1,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     4,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     2,     3,     4,     3,     3,     4,     1,
       3,     1,     1,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (pp, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, pp); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, struct ParserState *pp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (pp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, struct ParserState *pp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, pp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, struct ParserState *pp)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], pp);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, pp); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, struct ParserState *pp)
{
  YY_USE (yyvaluep);
  YY_USE (pp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (struct ParserState *pp)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, the_scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* unit: top_level_declaration  */
#line 147 "parse_buildast.y"
   { pp->parse_tree = (yyval.node) = new Node(AST_UNIT, {(yyvsp[0].node)}); }
#line 1471 "parse.tab.cpp"
    break;

  case 3: /* unit: top_level_declaration unit  */
#line 149 "parse_buildast.y"
    { pp->parse_tree = (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1477 "parse.tab.cpp"
    break;

  case 4: /* top_level_declaration: function_or_variable_declaration_or_definition  */
#line 154 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1483 "parse.tab.cpp"
    break;

  case 5: /* top_level_declaration: TOK_STATIC function_or_variable_declaration_or_definition  */
#line 156 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->shift_kid(); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1489 "parse.tab.cpp"
    break;

  case 6: /* top_level_declaration: TOK_EXTERN function_or_variable_declaration_or_definition  */
#line 158 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->shift_kid(); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1495 "parse.tab.cpp"
    break;

  case 7: /* top_level_declaration: struct_type_definition  */
#line 160 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1501 "parse.tab.cpp"
    break;

  case 8: /* top_level_declaration: union_type_definition  */
#line 162 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1507 "parse.tab.cpp"
    break;

  case 9: /* function_or_variable_declaration_or_definition: function_definition_or_declaration  */
#line 167 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1513 "parse.tab.cpp"
    break;

  case 10: /* function_or_variable_declaration_or_definition: simple_variable_declaration  */
#line 169 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1519 "parse.tab.cpp"
    break;

  case 11: /* simple_variable_declaration: type declarator_list TOK_SEMICOLON  */
#line 174 "parse_buildast.y"
    { (yyval.node) = new Node(AST_VARIABLE_DECLARATION, {(yyvsp[-2].node), (yyvsp[-1].node)}); handle_unspecified_storage((yyval.node), pp);  }
#line 1525 "parse.tab.cpp"
    break;

  case 12: /* declarator_list: declarator  */
#line 179 "parse_buildast.y"
    { (yyval.node) = new Node(AST_DECLARATOR_LIST, {(yyvsp[0].node)}); }
#line 1531 "parse.tab.cpp"
    break;

  case 13: /* declarator_list: declarator TOK_COMMA declarator_list  */
#line 181 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-2].node)); }
#line 1537 "parse.tab.cpp"
    break;

  case 14: /* declarator: TOK_ASTERISK declarator  */
#line 187 "parse_buildast.y"
    { (yyval.node) = new Node(AST_POINTER_DECLARATOR, {(yyvsp[0].node)}); }
#line 1543 "parse.tab.cpp"
    break;

  case 15: /* declarator: non_pointer_declarator  */
#line 189 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1549 "parse.tab.cpp"
    break;

  case 16: /* non_pointer_declarator: TOK_IDENT  */
#line 195 "parse_buildast.y"
    { (yyval.node) = new Node(AST_NAMED_DECLARATOR, {(yyvsp[0].node)}); }
#line 1555 "parse.tab.cpp"
    break;

  case 17: /* non_pointer_declarator: non_pointer_declarator TOK_LBRACKET TOK_INT_LIT TOK_RBRACKET  */
#line 197 "parse_buildast.y"
    { (yyval.node) = new Node(AST_ARRAY_DECLARATOR, {(yyvsp[-3].node), (yyvsp[-1].node)}); }
#line 1561 "parse.tab.cpp"
    break;

  case 18: /* function_definition_or_declaration: type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_LBRACE opt_statement_list TOK_RBRACE  */
#line 202 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_DEFINITION, {(yyvsp[-7].node), (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-1].node)}); }
#line 1567 "parse.tab.cpp"
    break;

  case 19: /* function_definition_or_declaration: type TOK_IDENT TOK_LPAREN function_parameter_list TOK_RPAREN TOK_SEMICOLON  */
#line 204 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_DECLARATION, {(yyvsp[-5].node), (yyvsp[-4].node), (yyvsp[-2].node)}); }
#line 1573 "parse.tab.cpp"
    break;

  case 20: /* function_parameter_list: TOK_VOID  */
#line 209 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_PARAMETER_LIST); }
#line 1579 "parse.tab.cpp"
    break;

  case 21: /* function_parameter_list: opt_parameter_list  */
#line 211 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1585 "parse.tab.cpp"
    break;

  case 22: /* opt_parameter_list: parameter_list  */
#line 216 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1591 "parse.tab.cpp"
    break;

  case 23: /* opt_parameter_list: %empty  */
#line 218 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_PARAMETER_LIST); }
#line 1597 "parse.tab.cpp"
    break;

  case 24: /* parameter_list: parameter  */
#line 223 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_PARAMETER_LIST, {(yyvsp[0].node)}); }
#line 1603 "parse.tab.cpp"
    break;

  case 25: /* parameter_list: parameter TOK_COMMA parameter_list  */
#line 225 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-2].node)); }
#line 1609 "parse.tab.cpp"
    break;

  case 26: /* parameter: type declarator  */
#line 230 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_PARAMETER, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 1615 "parse.tab.cpp"
    break;

  case 27: /* type: basic_type  */
#line 235 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1621 "parse.tab.cpp"
    break;

  case 28: /* type: TOK_STRUCT TOK_IDENT  */
#line 237 "parse_buildast.y"
    { (yyval.node) = new Node(AST_STRUCT_TYPE, {(yyvsp[0].node)}); }
#line 1627 "parse.tab.cpp"
    break;

  case 29: /* type: TOK_UNION TOK_IDENT  */
#line 239 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNION_TYPE, {(yyvsp[0].node)}); }
#line 1633 "parse.tab.cpp"
    break;

  case 30: /* basic_type: basic_type_keyword  */
#line 249 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BASIC_TYPE, {(yyvsp[0].node)}); }
#line 1639 "parse.tab.cpp"
    break;

  case 31: /* basic_type: basic_type_keyword basic_type  */
#line 251 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1645 "parse.tab.cpp"
    break;

  case 32: /* basic_type_keyword: TOK_CHAR  */
#line 256 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1651 "parse.tab.cpp"
    break;

  case 33: /* basic_type_keyword: TOK_SHORT  */
#line 258 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1657 "parse.tab.cpp"
    break;

  case 34: /* basic_type_keyword: TOK_INT  */
#line 260 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1663 "parse.tab.cpp"
    break;

  case 35: /* basic_type_keyword: TOK_LONG  */
#line 262 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1669 "parse.tab.cpp"
    break;

  case 36: /* basic_type_keyword: TOK_UNSIGNED  */
#line 264 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1675 "parse.tab.cpp"
    break;

  case 37: /* basic_type_keyword: TOK_SIGNED  */
#line 266 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1681 "parse.tab.cpp"
    break;

  case 38: /* basic_type_keyword: TOK_FLOAT  */
#line 268 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1687 "parse.tab.cpp"
    break;

  case 39: /* basic_type_keyword: TOK_DOUBLE  */
#line 270 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1693 "parse.tab.cpp"
    break;

  case 40: /* basic_type_keyword: TOK_VOID  */
#line 272 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1699 "parse.tab.cpp"
    break;

  case 41: /* basic_type_keyword: TOK_CONST  */
#line 274 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1705 "parse.tab.cpp"
    break;

  case 42: /* basic_type_keyword: TOK_VOLATILE  */
#line 276 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1711 "parse.tab.cpp"
    break;

  case 43: /* opt_statement_list: statement_list  */
#line 281 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1717 "parse.tab.cpp"
    break;

  case 44: /* opt_statement_list: %empty  */
#line 283 "parse_buildast.y"
    { (yyval.node) = new Node(AST_STATEMENT_LIST); }
#line 1723 "parse.tab.cpp"
    break;

  case 45: /* statement_list: statement  */
#line 288 "parse_buildast.y"
    { (yyval.node) = new Node(AST_STATEMENT_LIST, {(yyvsp[0].node)}); }
#line 1729 "parse.tab.cpp"
    break;

  case 46: /* statement_list: statement statement_list  */
#line 290 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1735 "parse.tab.cpp"
    break;

  case 47: /* statement: TOK_SEMICOLON  */
#line 295 "parse_buildast.y"
    { (yyval.node) = new Node(AST_EMPTY_STATEMENT); }
#line 1741 "parse.tab.cpp"
    break;

  case 48: /* statement: simple_variable_declaration  */
#line 297 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1747 "parse.tab.cpp"
    break;

  case 49: /* statement: TOK_STATIC simple_variable_declaration  */
#line 299 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->shift_kid(); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1753 "parse.tab.cpp"
    break;

  case 50: /* statement: TOK_EXTERN simple_variable_declaration  */
#line 301 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->shift_kid(); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1759 "parse.tab.cpp"
    break;

  case 51: /* statement: assignment_expression TOK_SEMICOLON  */
#line 303 "parse_buildast.y"
    { (yyval.node) = new Node(AST_EXPRESSION_STATEMENT, {(yyvsp[-1].node)}); }
#line 1765 "parse.tab.cpp"
    break;

  case 52: /* statement: TOK_RETURN TOK_SEMICOLON  */
#line 305 "parse_buildast.y"
    { (yyval.node) = new Node(AST_RETURN_STATEMENT); }
#line 1771 "parse.tab.cpp"
    break;

  case 53: /* statement: TOK_RETURN assignment_expression TOK_SEMICOLON  */
#line 307 "parse_buildast.y"
    { (yyval.node) = new Node(AST_RETURN_EXPRESSION_STATEMENT, {(yyvsp[-1].node)}); }
#line 1777 "parse.tab.cpp"
    break;

  case 54: /* statement: TOK_LBRACE opt_statement_list TOK_RBRACE  */
#line 309 "parse_buildast.y"
    { (yyval.node) = (yyvsp[-1].node);  }
#line 1783 "parse.tab.cpp"
    break;

  case 55: /* statement: TOK_WHILE TOK_LPAREN assignment_expression TOK_RPAREN statement  */
#line 311 "parse_buildast.y"
    { (yyval.node) = new Node(AST_WHILE_STATEMENT, {(yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1789 "parse.tab.cpp"
    break;

  case 56: /* statement: TOK_DO statement TOK_WHILE TOK_LPAREN assignment_expression TOK_RPAREN TOK_SEMICOLON  */
#line 313 "parse_buildast.y"
    { (yyval.node) = new Node(AST_DO_WHILE_STATEMENT, {(yyvsp[-5].node), (yyvsp[-2].node)}); }
#line 1795 "parse.tab.cpp"
    break;

  case 57: /* statement: TOK_FOR TOK_LPAREN assignment_expression TOK_SEMICOLON assignment_expression TOK_SEMICOLON assignment_expression TOK_RPAREN statement  */
#line 322 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FOR_STATEMENT, {(yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1801 "parse.tab.cpp"
    break;

  case 58: /* statement: TOK_IF TOK_LPAREN assignment_expression TOK_RPAREN statement  */
#line 324 "parse_buildast.y"
    { (yyval.node) = new Node(AST_IF_STATEMENT, {(yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1807 "parse.tab.cpp"
    break;

  case 59: /* statement: TOK_IF TOK_LPAREN assignment_expression TOK_RPAREN statement TOK_ELSE statement  */
#line 326 "parse_buildast.y"
    { (yyval.node) = new Node(AST_IF_ELSE_STATEMENT, {(yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1813 "parse.tab.cpp"
    break;

  case 60: /* struct_type_definition: TOK_STRUCT TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE TOK_SEMICOLON  */
#line 331 "parse_buildast.y"
    { (yyval.node) = new Node(AST_STRUCT_TYPE_DEFINITION, {(yyvsp[-4].node), (yyvsp[-2].node)}); }
#line 1819 "parse.tab.cpp"
    break;

  case 61: /* union_type_definition: TOK_UNION TOK_IDENT TOK_LBRACE opt_simple_variable_declaration_list TOK_RBRACE TOK_SEMICOLON  */
#line 336 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNION_TYPE_DEFINITION, {(yyvsp[-4].node), (yyvsp[-2].node)}); }
#line 1825 "parse.tab.cpp"
    break;

  case 62: /* opt_simple_variable_declaration_list: simple_variable_declaration_list  */
#line 341 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1831 "parse.tab.cpp"
    break;

  case 63: /* opt_simple_variable_declaration_list: %empty  */
#line 343 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FIELD_DEFINITION_LIST); }
#line 1837 "parse.tab.cpp"
    break;

  case 64: /* simple_variable_declaration_list: simple_variable_declaration  */
#line 348 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FIELD_DEFINITION_LIST, {(yyvsp[0].node)}); }
#line 1843 "parse.tab.cpp"
    break;

  case 65: /* simple_variable_declaration_list: simple_variable_declaration simple_variable_declaration_list  */
#line 350 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-1].node)); }
#line 1849 "parse.tab.cpp"
    break;

  case 66: /* assignment_expression: unary_expression assignment_op assignment_expression  */
#line 369 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1855 "parse.tab.cpp"
    break;

  case 67: /* assignment_expression: conditional_expression  */
#line 371 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1861 "parse.tab.cpp"
    break;

  case 68: /* assignment_op: TOK_ASSIGN  */
#line 376 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1867 "parse.tab.cpp"
    break;

  case 69: /* assignment_op: TOK_MUL_ASSIGN  */
#line 378 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1873 "parse.tab.cpp"
    break;

  case 70: /* assignment_op: TOK_DIV_ASSIGN  */
#line 380 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1879 "parse.tab.cpp"
    break;

  case 71: /* assignment_op: TOK_MOD_ASSIGN  */
#line 382 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1885 "parse.tab.cpp"
    break;

  case 72: /* assignment_op: TOK_ADD_ASSIGN  */
#line 384 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1891 "parse.tab.cpp"
    break;

  case 73: /* assignment_op: TOK_SUB_ASSIGN  */
#line 386 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1897 "parse.tab.cpp"
    break;

  case 74: /* assignment_op: TOK_LEFT_ASSIGN  */
#line 388 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1903 "parse.tab.cpp"
    break;

  case 75: /* assignment_op: TOK_RIGHT_ASSIGN  */
#line 390 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1909 "parse.tab.cpp"
    break;

  case 76: /* assignment_op: TOK_AND_ASSIGN  */
#line 392 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1915 "parse.tab.cpp"
    break;

  case 77: /* assignment_op: TOK_XOR_ASSIGN  */
#line 394 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1921 "parse.tab.cpp"
    break;

  case 78: /* assignment_op: TOK_OR_ASSIGN  */
#line 396 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1927 "parse.tab.cpp"
    break;

  case 79: /* conditional_expression: logical_or_expression  */
#line 401 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1933 "parse.tab.cpp"
    break;

  case 80: /* conditional_expression: logical_or_expression TOK_QUESTION assignment_expression TOK_COLON conditional_expression  */
#line 403 "parse_buildast.y"
    { (yyval.node) = new Node(AST_CONDITIONAL_EXPRESSION, {(yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1939 "parse.tab.cpp"
    break;

  case 81: /* logical_or_expression: logical_and_expression  */
#line 408 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1945 "parse.tab.cpp"
    break;

  case 82: /* logical_or_expression: logical_or_expression TOK_LOGICAL_OR logical_and_expression  */
#line 410 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1951 "parse.tab.cpp"
    break;

  case 83: /* logical_and_expression: bitwise_or_expression  */
#line 415 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1957 "parse.tab.cpp"
    break;

  case 84: /* logical_and_expression: logical_and_expression TOK_LOGICAL_AND bitwise_or_expression  */
#line 417 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1963 "parse.tab.cpp"
    break;

  case 85: /* bitwise_or_expression: bitwise_xor_expression  */
#line 422 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1969 "parse.tab.cpp"
    break;

  case 86: /* bitwise_or_expression: bitwise_or_expression TOK_BITWISE_OR bitwise_xor_expression  */
#line 424 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1975 "parse.tab.cpp"
    break;

  case 87: /* bitwise_xor_expression: bitwise_and_expression  */
#line 429 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1981 "parse.tab.cpp"
    break;

  case 88: /* bitwise_xor_expression: bitwise_xor_expression TOK_BITWISE_XOR bitwise_and_expression  */
#line 431 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1987 "parse.tab.cpp"
    break;

  case 89: /* bitwise_and_expression: equality_expression  */
#line 436 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1993 "parse.tab.cpp"
    break;

  case 90: /* bitwise_and_expression: bitwise_and_expression TOK_AMPERSAND equality_expression  */
#line 438 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 1999 "parse.tab.cpp"
    break;

  case 91: /* equality_expression: relational_expression  */
#line 443 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2005 "parse.tab.cpp"
    break;

  case 92: /* equality_expression: equality_expression TOK_EQUALITY relational_expression  */
#line 445 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2011 "parse.tab.cpp"
    break;

  case 93: /* equality_expression: equality_expression TOK_INEQUALITY relational_expression  */
#line 447 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2017 "parse.tab.cpp"
    break;

  case 94: /* relational_expression: shift_expression  */
#line 452 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2023 "parse.tab.cpp"
    break;

  case 95: /* relational_expression: relational_expression relational_op shift_expression  */
#line 454 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2029 "parse.tab.cpp"
    break;

  case 96: /* relational_op: TOK_LT  */
#line 459 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2035 "parse.tab.cpp"
    break;

  case 97: /* relational_op: TOK_LTE  */
#line 461 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2041 "parse.tab.cpp"
    break;

  case 98: /* relational_op: TOK_GT  */
#line 463 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2047 "parse.tab.cpp"
    break;

  case 99: /* relational_op: TOK_GTE  */
#line 465 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2053 "parse.tab.cpp"
    break;

  case 100: /* shift_expression: additive_expression  */
#line 470 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2059 "parse.tab.cpp"
    break;

  case 101: /* shift_expression: shift_expression TOK_LEFT_SHIFT additive_expression  */
#line 472 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2065 "parse.tab.cpp"
    break;

  case 102: /* shift_expression: shift_expression TOK_RIGHT_SHIFT additive_expression  */
#line 474 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2071 "parse.tab.cpp"
    break;

  case 103: /* additive_expression: multiplicative_expression  */
#line 479 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2077 "parse.tab.cpp"
    break;

  case 104: /* additive_expression: additive_expression TOK_PLUS multiplicative_expression  */
#line 481 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2083 "parse.tab.cpp"
    break;

  case 105: /* additive_expression: additive_expression TOK_MINUS multiplicative_expression  */
#line 483 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2089 "parse.tab.cpp"
    break;

  case 106: /* multiplicative_expression: cast_expression  */
#line 488 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2095 "parse.tab.cpp"
    break;

  case 107: /* multiplicative_expression: multiplicative_expression TOK_ASTERISK cast_expression  */
#line 490 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2101 "parse.tab.cpp"
    break;

  case 108: /* multiplicative_expression: multiplicative_expression TOK_DIVIDE cast_expression  */
#line 492 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2107 "parse.tab.cpp"
    break;

  case 109: /* multiplicative_expression: multiplicative_expression TOK_MOD cast_expression  */
#line 494 "parse_buildast.y"
    { (yyval.node) = new Node(AST_BINARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2113 "parse.tab.cpp"
    break;

  case 110: /* cast_expression: unary_expression  */
#line 499 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2119 "parse.tab.cpp"
    break;

  case 111: /* cast_expression: TOK_LPAREN type TOK_RPAREN cast_expression  */
#line 501 "parse_buildast.y"
    { (yyval.node) = new Node(AST_CAST_EXPRESSION, {(yyvsp[-3].node), (yyvsp[-2].node)}); }
#line 2125 "parse.tab.cpp"
    break;

  case 112: /* unary_expression: postfix_expression  */
#line 506 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2131 "parse.tab.cpp"
    break;

  case 113: /* unary_expression: TOK_PLUS cast_expression  */
#line 508 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2137 "parse.tab.cpp"
    break;

  case 114: /* unary_expression: TOK_MINUS cast_expression  */
#line 510 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2143 "parse.tab.cpp"
    break;

  case 115: /* unary_expression: TOK_NOT cast_expression  */
#line 512 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2149 "parse.tab.cpp"
    break;

  case 116: /* unary_expression: TOK_BITWISE_COMPL cast_expression  */
#line 514 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2155 "parse.tab.cpp"
    break;

  case 117: /* unary_expression: TOK_INCREMENT unary_expression  */
#line 516 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2161 "parse.tab.cpp"
    break;

  case 118: /* unary_expression: TOK_DECREMENT unary_expression  */
#line 518 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2167 "parse.tab.cpp"
    break;

  case 119: /* unary_expression: TOK_ASTERISK unary_expression  */
#line 520 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2173 "parse.tab.cpp"
    break;

  case 120: /* unary_expression: TOK_AMPERSAND unary_expression  */
#line 522 "parse_buildast.y"
    { (yyval.node) = new Node(AST_UNARY_EXPRESSION, {(yyvsp[-1].node), (yyvsp[0].node)}); }
#line 2179 "parse.tab.cpp"
    break;

  case 121: /* postfix_expression: primary_expression  */
#line 535 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2185 "parse.tab.cpp"
    break;

  case 122: /* postfix_expression: postfix_expression TOK_INCREMENT  */
#line 537 "parse_buildast.y"
    { (yyval.node) = new Node(AST_POSTFIX_EXPRESSION, {(yyvsp[0].node), (yyvsp[-1].node)}); }
#line 2191 "parse.tab.cpp"
    break;

  case 123: /* postfix_expression: postfix_expression TOK_DECREMENT  */
#line 539 "parse_buildast.y"
    { (yyval.node) = new Node(AST_POSTFIX_EXPRESSION, {(yyvsp[0].node), (yyvsp[-1].node)}); }
#line 2197 "parse.tab.cpp"
    break;

  case 124: /* postfix_expression: postfix_expression TOK_LPAREN TOK_RPAREN  */
#line 541 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_CALL_EXPRESSION, {(yyvsp[-2].node), new Node(AST_ARGUMENT_EXPRESSION_LIST)}); }
#line 2203 "parse.tab.cpp"
    break;

  case 125: /* postfix_expression: postfix_expression TOK_LPAREN argument_expression_list TOK_RPAREN  */
#line 543 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FUNCTION_CALL_EXPRESSION, {(yyvsp[-3].node), (yyvsp[-1].node)}); }
#line 2209 "parse.tab.cpp"
    break;

  case 126: /* postfix_expression: postfix_expression TOK_DOT TOK_IDENT  */
#line 545 "parse_buildast.y"
    { (yyval.node) = new Node(AST_FIELD_REF_EXPRESSION, {(yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2215 "parse.tab.cpp"
    break;

  case 127: /* postfix_expression: postfix_expression TOK_ARROW TOK_IDENT  */
#line 547 "parse_buildast.y"
    { (yyval.node) = new Node(AST_INDIRECT_FIELD_REF_EXPRESSION, {(yyvsp[-2].node), (yyvsp[0].node)}); }
#line 2221 "parse.tab.cpp"
    break;

  case 128: /* postfix_expression: postfix_expression TOK_LBRACKET assignment_expression TOK_RBRACKET  */
#line 549 "parse_buildast.y"
    { (yyval.node) = new Node(AST_ARRAY_ELEMENT_REF_EXPRESSION, {(yyvsp[-3].node), (yyvsp[-1].node)}); }
#line 2227 "parse.tab.cpp"
    break;

  case 129: /* argument_expression_list: assignment_expression  */
#line 554 "parse_buildast.y"
    { (yyval.node) = new Node(AST_ARGUMENT_EXPRESSION_LIST, {(yyvsp[0].node)}); }
#line 2233 "parse.tab.cpp"
    break;

  case 130: /* argument_expression_list: assignment_expression TOK_COMMA argument_expression_list  */
#line 556 "parse_buildast.y"
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->prepend_kid((yyvsp[-2].node)); }
#line 2239 "parse.tab.cpp"
    break;

  case 131: /* primary_expression: TOK_INT_LIT  */
#line 561 "parse_buildast.y"
    { (yyval.node) = new Node(AST_LITERAL_VALUE, {(yyvsp[0].node)}); }
#line 2245 "parse.tab.cpp"
    break;

  case 132: /* primary_expression: TOK_CHAR_LIT  */
#line 563 "parse_buildast.y"
    { (yyval.node) = new Node(AST_LITERAL_VALUE, {(yyvsp[0].node)}); }
#line 2251 "parse.tab.cpp"
    break;

  case 133: /* primary_expression: TOK_FP_LIT  */
#line 565 "parse_buildast.y"
    { (yyval.node) = new Node(AST_LITERAL_VALUE, {(yyvsp[0].node)}); }
#line 2257 "parse.tab.cpp"
    break;

  case 134: /* primary_expression: TOK_STR_LIT  */
#line 567 "parse_buildast.y"
    { (yyval.node) = new Node(AST_LITERAL_VALUE, {(yyvsp[0].node)}); }
#line 2263 "parse.tab.cpp"
    break;

  case 135: /* primary_expression: TOK_IDENT  */
#line 569 "parse_buildast.y"
    { (yyval.node) = new Node(AST_VARIABLE_REF, {(yyvsp[0].node)}); }
#line 2269 "parse.tab.cpp"
    break;

  case 136: /* primary_expression: TOK_LPAREN assignment_expression TOK_RPAREN  */
#line 571 "parse_buildast.y"
    { (yyval.node) = (yyvsp[-1].node); }
#line 2275 "parse.tab.cpp"
    break;


#line 2279 "parse.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (pp, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, pp);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, pp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (pp, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, pp);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, pp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 574 "parse_buildast.y"

