
/*  A Bison parser, made from jamgram.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	_BANG	257
#define	_BANG_EQUALS	258
#define	_AMPERAMPER	259
#define	_LPAREN	260
#define	_RPAREN	261
#define	_PLUS_EQUALS	262
#define	_COLON	263
#define	_SEMIC	264
#define	_LANGLE	265
#define	_LANGLE_EQUALS	266
#define	_EQUALS	267
#define	_RANGLE	268
#define	_RANGLE_EQUALS	269
#define	_QUESTION_EQUALS	270
#define	_LBRACKET	271
#define	_RBRACKET	272
#define	ACTIONS	273
#define	BIND	274
#define	CASE	275
#define	DEFAULT	276
#define	ELSE	277
#define	EXISTING	278
#define	FOR	279
#define	IF	280
#define	IGNORE	281
#define	IN	282
#define	INCLUDE	283
#define	LOCAL	284
#define	MODULE	285
#define	ON	286
#define	PIECEMEAL	287
#define	QUIETLY	288
#define	RETURN	289
#define	RULE	290
#define	SWITCH	291
#define	TOGETHER	292
#define	UPDATED	293
#define	WHILE	294
#define	_LBRACE	295
#define	_BARBAR	296
#define	_RBRACE	297
#define	ARG	298
#define	STRING	299


#include "jam.h"

#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "compile.h"
#include "newstr.h"

# define F0 (LIST *(*)(PARSE *, FRAME *))0
# define P0 (PARSE *)0
# define S0 (char *)0

# define pappend( l,r )    	parse_make( compile_append,l,r,P0,S0,S0,0 )
# define pfor( s,l,r,x )    	parse_make( compile_foreach,l,r,P0,s,S0,x )
# define pif( l,r,t )	  	parse_make( compile_if,l,r,t,S0,S0,0 )
# define pwhile( l,r )	  	parse_make( compile_while,l,r,P0,S0,S0,0 )
# define pincl( l )       	parse_make( compile_include,l,P0,P0,S0,S0,0 )
# define plist( s )	  	parse_make( compile_list,P0,P0,P0,s,S0,0 )
# define plocal( l,r,t )  	parse_make( compile_local,l,r,t,S0,S0,0 )
# define pmodule( l,r )	  	parse_make( compile_module,l,r,P0,S0,S0,0 )
# define pnull()	  	parse_make( compile_null,P0,P0,P0,S0,S0,0 )
# define prule( s,p )     	parse_make( compile_rule,p,P0,P0,s,S0,0 )
# define prules( l,r )	  	parse_make( compile_rules,l,r,P0,S0,S0,0 )
# define pset( l,r,a )          parse_make( compile_set,l,r,P0,S0,S0,a )
# define psetmodule( l,r ) 	parse_make( compile_set_module,l,r,P0,S0,S0,0 )
# define pset1( l,r,t,a )	parse_make( compile_settings,l,r,t,S0,S0,a )
# define psetc( s,p )     	parse_make( compile_setcomp,p,P0,P0,s,S0,0 )
# define psetc_args( s,p,a )    parse_make( compile_setcomp,p,a,P0,s,S0,0 )
# define psete( s,l,s1,f ) 	parse_make( compile_setexec,l,P0,P0,s,s1,f )
# define pswitch( l,r )   	parse_make( compile_switch,l,r,P0,S0,S0,0 )

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define pcnode( c,l,r )	parse_make( F0,l,r,P0,S0,S0,c )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		145
#define	YYFLAG		-32768
#define	YYNTBASE	46

#define YYTRANSLATE(x) ((unsigned)(x) <= 299 ? yytranslate[x] : 64)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     4,     6,     8,    11,    17,    18,    21,
    25,    29,    33,    38,    44,    51,    55,    63,    72,    78,
    84,    90,    96,   104,   111,   115,   116,   117,   127,   129,
   131,   133,   136,   138,   142,   146,   150,   154,   158,   162,
   166,   169,   173,   177,   181,   182,   185,   190,   192,   196,
   198,   199,   202,   204,   209,   210,   213,   215,   217,   219,
   221,   223,   225,   226
};

static const short yyrhs[] = {    -1,
    48,     0,     0,    48,     0,    50,     0,    50,    48,     0,
    30,    58,    49,    10,    47,     0,     0,    13,    58,     0,
    41,    47,    43,     0,    29,    58,    10,     0,    44,    57,
    10,     0,    60,    53,    58,    10,     0,    31,    30,    58,
    49,    10,     0,    60,    32,    58,    53,    58,    10,     0,
    35,    58,    10,     0,    25,    44,    28,    58,    41,    47,
    43,     0,    25,    30,    44,    28,    58,    41,    47,    43,
     0,    37,    58,    41,    55,    43,     0,    26,    54,    41,
    47,    43,     0,    31,    58,    41,    47,    43,     0,    40,
    54,    41,    47,    43,     0,    26,    54,    41,    47,    43,
    23,    50,     0,    36,    44,     6,    57,     7,    50,     0,
    36,    44,    50,     0,     0,     0,    19,    61,    44,    63,
    41,    51,    45,    52,    43,     0,    13,     0,     8,     0,
    16,     0,    22,    13,     0,    60,     0,    60,    13,    60,
     0,    60,     4,    60,     0,    60,    11,    60,     0,    60,
    12,    60,     0,    60,    14,    60,     0,    60,    15,    60,
     0,    60,    28,    58,     0,     3,    54,     0,    54,     5,
    54,     0,    54,    42,    54,     0,     6,    54,     7,     0,
     0,    56,    55,     0,    21,    44,     9,    47,     0,    58,
     0,    58,     9,    57,     0,    59,     0,     0,    59,    60,
     0,    44,     0,    17,    44,    57,    18,     0,     0,    61,
    62,     0,    39,     0,    38,     0,    27,     0,    34,     0,
    33,     0,    24,     0,     0,    20,    58,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   125,   127,   138,   140,   144,   146,   148,   152,   154,   158,
   160,   162,   164,   166,   168,   170,   172,   174,   176,   178,
   180,   182,   184,   186,   188,   190,   193,   195,   202,   204,
   206,   208,   216,   218,   220,   222,   224,   226,   228,   230,
   232,   234,   236,   238,   248,   250,   254,   263,   265,   275,
   279,   281,   285,   287,   297,   299,   303,   305,   307,   309,
   311,   313,   322,   324
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","_BANG",
"_BANG_EQUALS","_AMPERAMPER","_LPAREN","_RPAREN","_PLUS_EQUALS","_COLON","_SEMIC",
"_LANGLE","_LANGLE_EQUALS","_EQUALS","_RANGLE","_RANGLE_EQUALS","_QUESTION_EQUALS",
"_LBRACKET","_RBRACKET","ACTIONS","BIND","CASE","DEFAULT","ELSE","EXISTING",
"FOR","IF","IGNORE","IN","INCLUDE","LOCAL","MODULE","ON","PIECEMEAL","QUIETLY",
"RETURN","RULE","SWITCH","TOGETHER","UPDATED","WHILE","_LBRACE","_BARBAR","_RBRACE",
"ARG","STRING","run","block","rules","assign_list_opt","rule","@1","@2","assign",
"cond","cases","case","lol","list","listp","arg","eflags","eflag","bindlist", NULL
};
#endif

static const short yyr1[] = {     0,
    46,    46,    47,    47,    48,    48,    48,    49,    49,    50,
    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
    50,    50,    50,    50,    50,    51,    52,    50,    53,    53,
    53,    53,    54,    54,    54,    54,    54,    54,    54,    54,
    54,    54,    54,    54,    55,    55,    56,    57,    57,    58,
    59,    59,    60,    60,    61,    61,    62,    62,    62,    62,
    62,    62,    63,    63
};

static const short yyr2[] = {     0,
     0,     1,     0,     1,     1,     2,     5,     0,     2,     3,
     3,     3,     4,     5,     6,     3,     7,     8,     5,     5,
     5,     5,     7,     6,     3,     0,     0,     9,     1,     1,
     1,     2,     1,     3,     3,     3,     3,     3,     3,     3,
     2,     3,     3,     3,     0,     2,     4,     1,     3,     1,
     0,     2,     1,     4,     0,     2,     1,     1,     1,     1,
     1,     1,     0,     2
};

static const short yydefact[] = {     1,
     0,    55,     0,     0,    51,    51,    51,    51,     0,    51,
     0,     3,    53,     2,     5,     0,    51,     0,     0,     0,
     0,     0,    53,     0,    33,     0,    50,     8,    51,     0,
     0,     0,     0,     0,     0,     4,     0,    48,     6,    30,
    29,    31,     0,    51,    51,     0,    62,    59,    61,    60,
    58,    57,    63,    56,     0,    51,    41,     0,     0,     3,
     0,     0,     0,     0,     0,     0,     0,    51,    11,    52,
    51,     0,     8,     3,    16,    51,    25,    45,     3,    10,
    12,    51,    32,     0,     0,    54,    51,     0,    51,     0,
    44,    42,     0,    43,    35,    36,    37,    34,    38,    39,
    40,     9,     3,     0,     0,     0,     0,     0,    45,     0,
    49,    51,    13,    64,    26,     0,     3,    20,     7,    14,
    21,     0,     0,    19,    46,    22,     0,     0,     3,     0,
     0,    24,     3,    15,    27,     0,    17,    23,    47,     0,
    18,    28,     0,     0,     0
};

static const short yydefgoto[] = {   143,
    35,    36,    72,    15,   128,   140,    45,    24,   108,   109,
    37,    38,    27,    16,    18,    54,    88
};

static const short yypact[] = {   106,
   -34,-32768,   -13,     8,-32768,-32768,   -17,-32768,   -16,-32768,
     8,   106,    24,-32768,   106,   174,-32768,    95,   -15,   -12,
     8,     8,-32768,     4,   166,    22,    -2,    23,-32768,    -6,
    28,    80,     2,    18,     1,-32768,    39,    45,-32768,-32768,
-32768,-32768,    42,-32768,-32768,    43,-32768,-32768,-32768,-32768,
-32768,-32768,    49,-32768,    34,-32768,-32768,    15,     8,   106,
     8,    -2,    -2,    -2,    -2,    -2,    -2,-32768,-32768,-32768,
-32768,    60,    23,   106,-32768,-32768,-32768,    52,   106,-32768,
-32768,-32768,-32768,    59,    64,-32768,-32768,    36,-32768,    37,
-32768,-32768,    40,    74,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   106,    85,    53,    91,    56,    58,    52,    61,
-32768,-32768,-32768,-32768,-32768,    67,   106,    79,-32768,-32768,
-32768,   132,   101,-32768,-32768,-32768,   102,    68,   106,    71,
   132,-32768,   106,-32768,-32768,    75,-32768,-32768,-32768,    83,
-32768,-32768,   127,   130,-32768
};

static const short yypgoto[] = {-32768,
   -53,    12,    65,   -28,-32768,-32768,    69,    -3,    31,-32768,
   -11,    -5,-32768,    26,-32768,-32768,-32768
};


#define	YYLAST		206


static const short yytable[] = {    26,
    28,    30,    31,    77,    33,    46,    93,    34,    59,    17,
    21,    14,    29,    22,     1,    56,    19,    57,    58,    59,
   105,    91,    59,    73,     1,   110,    39,    32,    55,    25,
    20,    69,   -51,   -51,    74,    71,    25,    75,    84,    85,
   -51,    23,    78,    80,    60,    61,    25,    25,    81,   119,
    90,    23,    70,    82,    83,    92,    61,    94,    79,    61,
    86,    89,   101,   130,   106,   102,    40,   -51,    87,   103,
   111,    41,   107,   113,    42,   136,   115,   117,    59,   139,
    43,   114,   118,   116,    25,    76,    25,    95,    96,    97,
    98,    99,   100,   132,   120,   121,     1,   122,     2,   123,
   124,   131,   138,   126,     3,     4,   127,   129,     5,   133,
     7,   134,   135,   137,     8,     9,    10,   141,    47,    11,
    12,    48,     1,    13,     2,   142,   144,    49,    50,   145,
     3,     4,    51,    52,     5,     6,     7,   104,    53,   125,
     8,     9,    10,     0,     0,    11,    12,     0,     1,    13,
     2,     0,   112,     0,     0,     0,     3,     4,     0,     0,
     5,     0,     7,     0,     0,     0,     8,     9,    10,    62,
     0,    11,    12,     0,     0,    13,    63,    64,    65,    66,
    67,    40,     0,     0,     0,     0,    41,     0,     0,    42,
     0,     0,     0,    68,     0,    43,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    44
};

static const short yycheck[] = {     5,
     6,     7,     8,    32,    10,    17,    60,    11,     5,    44,
     3,     0,    30,     6,    17,    28,    30,    21,    22,     5,
    74,     7,     5,    29,    17,    79,    15,    44,    44,     4,
    44,    10,     9,    10,    41,    13,    11,    10,    44,    45,
    17,    44,    41,    43,    41,    42,    21,    22,    10,   103,
    56,    44,    27,     9,    13,    59,    42,    61,    41,    42,
    18,    28,    68,   117,    76,    71,     8,    44,    20,    10,
    82,    13,    21,    10,    16,   129,    41,    41,     5,   133,
    22,    87,    43,    89,    59,     6,    61,    62,    63,    64,
    65,    66,    67,   122,    10,    43,    17,     7,    19,    44,
    43,    23,   131,    43,    25,    26,   112,    41,    29,     9,
    31,    10,    45,    43,    35,    36,    37,    43,    24,    40,
    41,    27,    17,    44,    19,    43,     0,    33,    34,     0,
    25,    26,    38,    39,    29,    30,    31,    73,    44,   109,
    35,    36,    37,    -1,    -1,    40,    41,    -1,    17,    44,
    19,    -1,    84,    -1,    -1,    -1,    25,    26,    -1,    -1,
    29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,     4,
    -1,    40,    41,    -1,    -1,    44,    11,    12,    13,    14,
    15,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    16,
    -1,    -1,    -1,    28,    -1,    22,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    32
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

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
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *yyssp++ = yystate;

  if (yyssp >= yyss + yystacksize)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
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
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
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

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 2:
{ parse_save( yyvsp[0].parse ); ;
    break;}
case 3:
{ yyval.parse = pnull(); ;
    break;}
case 4:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
case 5:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
case 6:
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 7:
{ yyval.parse = plocal( yyvsp[-3].parse, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 8:
{ yyval.parse = pnull(); ;
    break;}
case 9:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
case 10:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 11:
{ yyval.parse = pincl( yyvsp[-1].parse ); ;
    break;}
case 12:
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); ;
    break;}
case 13:
{ yyval.parse = pset( yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); ;
    break;}
case 14:
{ yyval.parse = psetmodule( yyvsp[-2].parse, yyvsp[-1].parse ); ;
    break;}
case 15:
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); ;
    break;}
case 16:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 17:
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, 0 ); ;
    break;}
case 18:
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, 1 ); ;
    break;}
case 19:
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); ;
    break;}
case 20:
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); ;
    break;}
case 21:
{ yyval.parse = pmodule( yyvsp[-3].parse, yyvsp[-1].parse ); ;
    break;}
case 22:
{ yyval.parse = pwhile( yyvsp[-3].parse, yyvsp[-1].parse ); ;
    break;}
case 23:
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); ;
    break;}
case 24:
{ yyval.parse = psetc_args( yyvsp[-4].string, yyvsp[0].parse, yyvsp[-2].parse ); ;
    break;}
case 25:
{ yyval.parse = psetc( yyvsp[-1].string, yyvsp[0].parse ); ;
    break;}
case 26:
{ yymode( SCAN_STRING ); ;
    break;}
case 27:
{ yymode( SCAN_NORMAL ); ;
    break;}
case 28:
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); ;
    break;}
case 29:
{ yyval.number = ASSIGN_SET; ;
    break;}
case 30:
{ yyval.number = ASSIGN_APPEND; ;
    break;}
case 31:
{ yyval.number = ASSIGN_DEFAULT; ;
    break;}
case 32:
{ yyval.number = ASSIGN_DEFAULT; ;
    break;}
case 33:
{ yyval.parse = pcnode( COND_EXISTS, yyvsp[0].parse, pnull() ); ;
    break;}
case 34:
{ yyval.parse = pcnode( COND_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 35:
{ yyval.parse = pcnode( COND_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 36:
{ yyval.parse = pcnode( COND_LESS, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 37:
{ yyval.parse = pcnode( COND_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 38:
{ yyval.parse = pcnode( COND_MORE, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 39:
{ yyval.parse = pcnode( COND_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 40:
{ yyval.parse = pcnode( COND_IN, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 41:
{ yyval.parse = pcnode( COND_NOT, yyvsp[0].parse, P0 ); ;
    break;}
case 42:
{ yyval.parse = pcnode( COND_AND, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 43:
{ yyval.parse = pcnode( COND_OR, yyvsp[-2].parse, yyvsp[0].parse ); ;
    break;}
case 44:
{ yyval.parse = yyvsp[-1].parse; ;
    break;}
case 45:
{ yyval.parse = P0; ;
    break;}
case 46:
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 47:
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); ;
    break;}
case 48:
{ yyval.parse = pnode( P0, yyvsp[0].parse ); ;
    break;}
case 49:
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); ;
    break;}
case 50:
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); ;
    break;}
case 51:
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); ;
    break;}
case 52:
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); ;
    break;}
case 53:
{ yyval.parse = plist( yyvsp[0].string ); ;
    break;}
case 54:
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); ;
    break;}
case 55:
{ yyval.number = 0; ;
    break;}
case 56:
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; ;
    break;}
case 57:
{ yyval.number = EXEC_UPDATED; ;
    break;}
case 58:
{ yyval.number = EXEC_TOGETHER; ;
    break;}
case 59:
{ yyval.number = EXEC_IGNORE; ;
    break;}
case 60:
{ yyval.number = EXEC_QUIETLY; ;
    break;}
case 61:
{ yyval.number = EXEC_PIECEMEAL; ;
    break;}
case 62:
{ yyval.number = EXEC_EXISTING; ;
    break;}
case 63:
{ yyval.parse = pnull(); ;
    break;}
case 64:
{ yyval.parse = yyvsp[0].parse; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *ssp1++);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *(yyssp - 1);
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *(yyssp - 1))
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp - 1 == yyss) YYABORT;
  yyvsp--;
  yyssp--;
  yystate = *(yyssp - 1);
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *ssp1++);
      fprintf (stderr, "\n");
    }
#endif

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

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}


