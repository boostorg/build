/* A Bison parser, made from jamgram.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	_BANG_t	257
# define	_BANG_EQUALS_t	258
# define	_AMPER_t	259
# define	_AMPERAMPER_t	260
# define	_LPAREN_t	261
# define	_RPAREN_t	262
# define	_PLUS_EQUALS_t	263
# define	_COLON_t	264
# define	_SEMIC_t	265
# define	_LANGLE_t	266
# define	_LANGLE_EQUALS_t	267
# define	_EQUALS_t	268
# define	_RANGLE_t	269
# define	_RANGLE_EQUALS_t	270
# define	_QUESTION_EQUALS_t	271
# define	_LBRACKET_t	272
# define	_RBRACKET_t	273
# define	ACTIONS_t	274
# define	BIND_t	275
# define	CASE_t	276
# define	DEFAULT_t	277
# define	ELSE_t	278
# define	EXISTING_t	279
# define	FOR_t	280
# define	IF_t	281
# define	IGNORE_t	282
# define	IN_t	283
# define	INCLUDE_t	284
# define	LOCAL_t	285
# define	MODULE_t	286
# define	ON_t	287
# define	PIECEMEAL_t	288
# define	QUIETLY_t	289
# define	RETURN_t	290
# define	RULE_t	291
# define	SWITCH_t	292
# define	TOGETHER_t	293
# define	UPDATED_t	294
# define	WHILE_t	295
# define	_LBRACE_t	296
# define	_BAR_t	297
# define	_BARBAR_t	298
# define	_RBRACE_t	299
# define	ARG	300
# define	STRING	301

#line 97 "jamgram.y"

#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "compile.h"
#include "newstr.h"
#include "rules.h"

# define YYMAXDEPTH 10000	/* for OSF and other less endowed yaccs */

# define F0 (LIST *(*)(PARSE *, FRAME *))0
# define P0 (PARSE *)0
# define S0 (char *)0

# define pappend( l,r )    	parse_make( compile_append,l,r,P0,S0,S0,0 )
# define peval( c,l,r )	parse_make( compile_eval,l,r,P0,S0,S0,c )
# define pfor( s,l,r,x )    	parse_make( compile_foreach,l,r,P0,s,S0,x )
# define pif( l,r,t )	  	parse_make( compile_if,l,r,t,S0,S0,0 )
# define pincl( l )       	parse_make( compile_include,l,P0,P0,S0,S0,0 )
# define plist( s )	  	parse_make( compile_list,P0,P0,P0,s,S0,0 )
# define plocal( l,r,t )  	parse_make( compile_local,l,r,t,S0,S0,0 )
# define pmodule( l,r )	  	parse_make( compile_module,l,r,P0,S0,S0,0 )
# define pnull()	  	parse_make( compile_null,P0,P0,P0,S0,S0,0 )
# define pon( l,r )	  	parse_make( compile_on,l,r,P0,S0,S0,0 )
# define prule( s,p )     	parse_make( compile_rule,p,P0,P0,s,S0,0 )
# define prules( l,r )	  	parse_make( compile_rules,l,r,P0,S0,S0,0 )
# define pset( l,r,a )          parse_make( compile_set,l,r,P0,S0,S0,a )
# define pset1( l,r,t,a )	parse_make( compile_settings,l,r,t,S0,S0,a )
# define psetc( s,p,a,l )     	parse_make( compile_setcomp,p,a,P0,s,S0,l )
# define psete( s,l,s1,f ) 	parse_make( compile_setexec,l,P0,P0,s,s1,f )
# define pswitch( l,r )   	parse_make( compile_switch,l,r,P0,S0,S0,0 )
# define pwhile( l,r )   	parse_make( compile_while,l,r,P0,S0,S0,0 )

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		154
#define	YYFLAG		-32768
#define	YYNTBASE	48

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 301 ? yytranslate[x] : 71)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     3,     5,     7,     9,    12,    18,    19,
      22,    24,    28,    29,    31,    32,    36,    40,    44,    49,
      56,    60,    69,    75,    81,    87,    93,   101,   107,   111,
     112,   113,   123,   125,   127,   129,   132,   134,   138,   142,
     146,   150,   154,   158,   162,   166,   170,   174,   178,   181,
     185,   186,   189,   194,   196,   200,   202,   203,   206,   208,
     209,   214,   217,   222,   227,   228,   231,   233,   235,   237,
     239,   241,   243,   244
};
static const short yyrhs[] =
{
      -1,    50,     0,    51,     0,    50,     0,    55,     0,    55,
      50,     0,    31,    63,    52,    11,    49,     0,     0,    14,
      63,     0,    51,     0,     7,    62,     8,     0,     0,    31,
       0,     0,    42,    49,    45,     0,    30,    63,    11,     0,
      46,    62,    11,     0,    65,    58,    63,    11,     0,    65,
      33,    63,    58,    63,    11,     0,    36,    63,    11,     0,
      26,    54,    46,    29,    63,    42,    49,    45,     0,    38,
      63,    42,    60,    45,     0,    27,    59,    42,    49,    45,
       0,    32,    63,    42,    49,    45,     0,    41,    59,    42,
      49,    45,     0,    27,    59,    42,    49,    45,    24,    55,
       0,    54,    37,    46,    53,    55,     0,    33,    65,    55,
       0,     0,     0,    20,    68,    46,    70,    42,    56,    47,
      57,    45,     0,    14,     0,     9,     0,    17,     0,    23,
      14,     0,    65,     0,    59,    14,    59,     0,    59,     4,
      59,     0,    59,    12,    59,     0,    59,    13,    59,     0,
      59,    15,    59,     0,    59,    16,    59,     0,    59,     5,
      59,     0,    59,     6,    59,     0,    59,    43,    59,     0,
      59,    44,    59,     0,    65,    29,    63,     0,     3,    59,
       0,     7,    59,     8,     0,     0,    61,    60,     0,    22,
      46,    10,    49,     0,    63,     0,    63,    10,    62,     0,
      64,     0,     0,    64,    65,     0,    46,     0,     0,    18,
      66,    67,    19,     0,    65,    62,     0,    33,    65,    65,
      62,     0,    33,    65,    36,    63,     0,     0,    68,    69,
       0,    40,     0,    39,     0,    28,     0,    35,     0,    34,
       0,    25,     0,     0,    21,    63,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   139,   141,   152,   154,   158,   160,   162,   166,   170,
     172,   176,   178,   182,   184,   188,   190,   192,   194,   196,
     198,   200,   202,   204,   206,   208,   210,   212,   214,   216,
     216,   216,   228,   230,   232,   234,   241,   243,   245,   247,
     249,   251,   253,   255,   257,   259,   261,   263,   265,   267,
     278,   280,   284,   293,   295,   305,   309,   311,   315,   317,
     317,   326,   328,   330,   340,   342,   346,   348,   350,   352,
     354,   356,   365,   367
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "_BANG_t", "_BANG_EQUALS_t", "_AMPER_t", 
  "_AMPERAMPER_t", "_LPAREN_t", "_RPAREN_t", "_PLUS_EQUALS_t", "_COLON_t", 
  "_SEMIC_t", "_LANGLE_t", "_LANGLE_EQUALS_t", "_EQUALS_t", "_RANGLE_t", 
  "_RANGLE_EQUALS_t", "_QUESTION_EQUALS_t", "_LBRACKET_t", "_RBRACKET_t", 
  "ACTIONS_t", "BIND_t", "CASE_t", "DEFAULT_t", "ELSE_t", "EXISTING_t", 
  "FOR_t", "IF_t", "IGNORE_t", "IN_t", "INCLUDE_t", "LOCAL_t", "MODULE_t", 
  "ON_t", "PIECEMEAL_t", "QUIETLY_t", "RETURN_t", "RULE_t", "SWITCH_t", 
  "TOGETHER_t", "UPDATED_t", "WHILE_t", "_LBRACE_t", "_BAR_t", 
  "_BARBAR_t", "_RBRACE_t", "ARG", "STRING", "run", "block", "rules", 
  "null", "assign_list_opt", "arglist_opt", "local_opt", "rule", "@1", 
  "@2", "assign", "expr", "cases", "case", "lol", "list", "listp", "arg", 
  "@3", "func", "eflags", "eflag", "bindlist", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    48,    48,    49,    49,    50,    50,    50,    51,    52,
      52,    53,    53,    54,    54,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    56,
      57,    55,    58,    58,    58,    58,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    60,    61,    62,    62,    63,    64,    64,    65,    66,
      65,    67,    67,    67,    68,    68,    69,    69,    69,    69,
      69,    69,    70,    70
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     1,     1,     1,     1,     2,     5,     0,     2,
       1,     3,     0,     1,     0,     3,     3,     3,     4,     6,
       3,     8,     5,     5,     5,     5,     7,     5,     3,     0,
       0,     9,     1,     1,     1,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       0,     2,     4,     1,     3,     1,     0,     2,     1,     0,
       4,     2,     4,     4,     0,     2,     1,     1,     1,     1,
       1,     1,     0,     2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,    59,    64,    14,     0,    56,    56,    56,     0,    56,
      56,     0,     8,    58,     2,     0,     5,     0,     0,     0,
      13,     0,     0,     0,    58,     0,    36,     0,    55,     8,
       0,    14,     0,     0,     0,     0,     4,     3,     0,    53,
       0,     6,    33,    32,    34,     0,    56,    56,     0,    56,
       0,    71,    68,    70,    69,    67,    66,    72,    65,     0,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       8,     0,     0,    56,    16,    57,    56,    10,     0,     8,
      28,    20,    50,     8,    15,    17,    56,    12,    35,     0,
       0,     0,    61,    60,    56,     0,    56,    49,    38,    43,
      44,    39,    40,    37,    41,    42,     0,    45,    46,    47,
       9,     8,     0,     0,     0,    50,     0,    54,    56,    14,
      56,    18,    56,    56,    73,    29,     0,    23,     7,    24,
       0,    22,    51,    25,     0,    27,     0,    63,    62,     0,
       8,    14,     8,    11,    19,    30,     0,    26,    52,     0,
      21,    31,     0,     0,     0
};

static const short yydefgoto[] =
{
     152,    35,    36,    37,    78,   119,    15,    16,   139,   149,
      47,    25,   114,   115,    38,    39,    28,    26,    18,    50,
      19,    58,    95
};

static const short yypact[] =
{
     136,-32768,-32768,   -14,     6,-32768,   -17,-32768,     3,-32768,
  -32768,     6,   136,     4,-32768,     1,   136,    44,    14,   170,
  -32768,    -6,     6,     6,-32768,    21,    15,    55,     3,    41,
      29,   161,    63,    33,   108,    36,-32768,-32768,    78,    83,
      48,-32768,-32768,-32768,-32768,    90,-32768,-32768,     3,-32768,
      88,-32768,-32768,-32768,-32768,-32768,-32768,    87,-32768,    80,
  -32768,   121,     6,     6,     6,     6,     6,     6,     6,     6,
     136,     6,     6,-32768,-32768,-32768,-32768,-32768,    99,   136,
  -32768,-32768,    93,   136,-32768,-32768,-32768,   123,-32768,    45,
     117,    -7,-32768,-32768,-32768,    89,-32768,-32768,   131,    72,
      72,-32768,-32768,   131,-32768,-32768,    94,   207,   207,-32768,
  -32768,   136,   100,    86,   103,    93,   104,-32768,-32768,   161,
  -32768,-32768,-32768,-32768,-32768,-32768,    96,   129,-32768,-32768,
     145,-32768,-32768,-32768,   149,-32768,   147,-32768,-32768,   112,
     136,   161,   136,-32768,-32768,-32768,   115,-32768,-32768,   116,
  -32768,-32768,   171,   175,-32768
};

static const short yypgoto[] =
{
  -32768,   -60,    30,   141,-32768,-32768,   173,   -24,-32768,-32768,
      91,    34,    68,-32768,   -45,    -4,-32768,     0,-32768,-32768,
  -32768,-32768,-32768
};


#define	YYLAST		223


static const short yytable[] =
{
      17,    27,    29,    30,    92,    32,    33,    80,    31,    22,
     106,     1,    17,    23,   -56,   -56,    17,    20,    49,   112,
     -13,     1,   -56,   116,     1,    62,    63,    64,    75,   122,
      14,    17,     1,    65,    66,    67,    68,    69,    40,    24,
      59,   117,    89,    90,    73,    34,    41,    48,    91,    24,
     -56,   128,    24,    42,    42,    76,    60,    61,    43,    43,
      24,    44,    44,    70,    71,    72,    74,    45,    45,   109,
      17,    79,   110,   134,    81,    82,    62,    46,   138,    17,
     146,    84,   148,    17,    65,    66,    67,    68,    69,    85,
     124,   123,   126,    86,    87,   135,    98,    99,   100,   101,
     102,   103,   104,   105,    88,   107,   108,    93,    94,    96,
     111,    17,    62,    63,    64,   113,   136,   147,   137,    17,
      65,    66,    67,    68,    69,    62,    63,    64,   121,    97,
     118,   125,   130,    65,    66,    67,    68,    69,   140,   127,
      17,    17,    17,    65,    66,   129,    68,    69,   131,   133,
      83,    71,    72,   141,     1,   142,     2,   143,   144,   145,
     150,   151,     3,     4,    71,    72,     5,     6,     7,     8,
      77,   153,     9,   -14,    10,   154,    21,    11,    12,     1,
     120,     2,    13,   132,     0,     0,     0,     3,     4,     0,
       0,     5,    20,     7,     8,    51,     0,     9,    52,    10,
       0,     0,    11,    12,    53,    54,     0,    13,     0,    55,
      56,    62,    63,    64,     0,     0,    57,     0,     0,    65,
      66,    67,    68,    69
};

static const short yycheck[] =
{
       0,     5,     6,     7,    49,     9,    10,    31,     8,     3,
      70,    18,    12,     7,    10,    11,    16,    31,    18,    79,
      37,    18,    18,    83,    18,     4,     5,     6,    28,    36,
       0,    31,    18,    12,    13,    14,    15,    16,    37,    46,
      46,    86,    46,    47,    29,    11,    16,    33,    48,    46,
      46,   111,    46,     9,     9,    14,    22,    23,    14,    14,
      46,    17,    17,    42,    43,    44,    11,    23,    23,    73,
      70,    42,    76,   118,    11,    42,     4,    33,   123,    79,
     140,    45,   142,    83,    12,    13,    14,    15,    16,    11,
      94,    91,    96,    10,    46,   119,    62,    63,    64,    65,
      66,    67,    68,    69,    14,    71,    72,    19,    21,    29,
      11,   111,     4,     5,     6,    22,   120,   141,   122,   119,
      12,    13,    14,    15,    16,     4,     5,     6,    11,     8,
       7,    42,    46,    12,    13,    14,    15,    16,    42,    45,
     140,   141,   142,    12,    13,    45,    15,    16,    45,    45,
      42,    43,    44,    24,    18,    10,    20,     8,    11,    47,
      45,    45,    26,    27,    43,    44,    30,    31,    32,    33,
      29,     0,    36,    37,    38,     0,     3,    41,    42,    18,
      89,    20,    46,   115,    -1,    -1,    -1,    26,    27,    -1,
      -1,    30,    31,    32,    33,    25,    -1,    36,    28,    38,
      -1,    -1,    41,    42,    34,    35,    -1,    46,    -1,    39,
      40,     4,     5,     6,    -1,    -1,    46,    -1,    -1,    12,
      13,    14,    15,    16
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 142 "jamgram.y"
{ parse_save( yyvsp[0].parse ); }
    break;
case 3:
#line 153 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
    break;
case 4:
#line 155 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
    break;
case 5:
#line 159 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
    break;
case 6:
#line 161 "jamgram.y"
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); }
    break;
case 7:
#line 163 "jamgram.y"
{ yyval.parse = plocal( yyvsp[-3].parse, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 8:
#line 167 "jamgram.y"
{ yyval.parse = pnull(); }
    break;
case 9:
#line 171 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_SET; }
    break;
case 10:
#line 173 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_APPEND; }
    break;
case 11:
#line 177 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
    break;
case 12:
#line 179 "jamgram.y"
{ yyval.parse = P0; }
    break;
case 13:
#line 183 "jamgram.y"
{ yyval.number = 1; }
    break;
case 14:
#line 185 "jamgram.y"
{ yyval.number = 0; }
    break;
case 15:
#line 189 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
    break;
case 16:
#line 191 "jamgram.y"
{ yyval.parse = pincl( yyvsp[-1].parse ); }
    break;
case 17:
#line 193 "jamgram.y"
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); }
    break;
case 18:
#line 195 "jamgram.y"
{ yyval.parse = pset( yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
    break;
case 19:
#line 197 "jamgram.y"
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
    break;
case 20:
#line 199 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
    break;
case 21:
#line 201 "jamgram.y"
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-6].number ); }
    break;
case 22:
#line 203 "jamgram.y"
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); }
    break;
case 23:
#line 205 "jamgram.y"
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); }
    break;
case 24:
#line 207 "jamgram.y"
{ yyval.parse = pmodule( yyvsp[-3].parse, yyvsp[-1].parse ); }
    break;
case 25:
#line 209 "jamgram.y"
{ yyval.parse = pwhile( yyvsp[-3].parse, yyvsp[-1].parse ); }
    break;
case 26:
#line 211 "jamgram.y"
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); }
    break;
case 27:
#line 213 "jamgram.y"
{ yyval.parse = psetc( yyvsp[-2].string, yyvsp[0].parse, yyvsp[-1].parse, yyvsp[-4].number ); }
    break;
case 28:
#line 215 "jamgram.y"
{ yyval.parse = pon( yyvsp[-1].parse, yyvsp[0].parse ); }
    break;
case 29:
#line 217 "jamgram.y"
{ yymode( SCAN_STRING ); }
    break;
case 30:
#line 219 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
    break;
case 31:
#line 221 "jamgram.y"
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); }
    break;
case 32:
#line 229 "jamgram.y"
{ yyval.number = ASSIGN_SET; }
    break;
case 33:
#line 231 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; }
    break;
case 34:
#line 233 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
    break;
case 35:
#line 235 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
    break;
case 36:
#line 242 "jamgram.y"
{ yyval.parse = peval( EXPR_EXISTS, yyvsp[0].parse, pnull() ); }
    break;
case 37:
#line 244 "jamgram.y"
{ yyval.parse = peval( EXPR_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 38:
#line 246 "jamgram.y"
{ yyval.parse = peval( EXPR_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 39:
#line 248 "jamgram.y"
{ yyval.parse = peval( EXPR_LESS, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 40:
#line 250 "jamgram.y"
{ yyval.parse = peval( EXPR_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 41:
#line 252 "jamgram.y"
{ yyval.parse = peval( EXPR_MORE, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 42:
#line 254 "jamgram.y"
{ yyval.parse = peval( EXPR_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 43:
#line 256 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 44:
#line 258 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 45:
#line 260 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 46:
#line 262 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 47:
#line 264 "jamgram.y"
{ yyval.parse = peval( EXPR_IN, yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 48:
#line 266 "jamgram.y"
{ yyval.parse = peval( EXPR_NOT, yyvsp[0].parse, pnull() ); }
    break;
case 49:
#line 268 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
    break;
case 50:
#line 279 "jamgram.y"
{ yyval.parse = P0; }
    break;
case 51:
#line 281 "jamgram.y"
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); }
    break;
case 52:
#line 285 "jamgram.y"
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); }
    break;
case 53:
#line 294 "jamgram.y"
{ yyval.parse = pnode( P0, yyvsp[0].parse ); }
    break;
case 54:
#line 296 "jamgram.y"
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); }
    break;
case 55:
#line 306 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); }
    break;
case 56:
#line 310 "jamgram.y"
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); }
    break;
case 57:
#line 312 "jamgram.y"
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); }
    break;
case 58:
#line 316 "jamgram.y"
{ yyval.parse = plist( yyvsp[0].string ); }
    break;
case 59:
#line 317 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
    break;
case 60:
#line 318 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
    break;
case 61:
#line 327 "jamgram.y"
{ yyval.parse = prule( yyvsp[-1].string, yyvsp[0].parse ); }
    break;
case 62:
#line 329 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, prule( yyvsp[-1].string, yyvsp[0].parse ) ); }
    break;
case 63:
#line 331 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, yyvsp[0].parse ); }
    break;
case 64:
#line 341 "jamgram.y"
{ yyval.number = 0; }
    break;
case 65:
#line 343 "jamgram.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
    break;
case 66:
#line 347 "jamgram.y"
{ yyval.number = EXEC_UPDATED; }
    break;
case 67:
#line 349 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; }
    break;
case 68:
#line 351 "jamgram.y"
{ yyval.number = EXEC_IGNORE; }
    break;
case 69:
#line 353 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; }
    break;
case 70:
#line 355 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; }
    break;
case 71:
#line 357 "jamgram.y"
{ yyval.number = EXEC_EXISTING; }
    break;
case 72:
#line 366 "jamgram.y"
{ yyval.parse = pnull(); }
    break;
case 73:
#line 368 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 372 "jamgram.y"
