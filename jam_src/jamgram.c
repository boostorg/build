# define _BANG_t 257
# define _BANG_EQUALS_t 258
# define _AMPER_t 259
# define _AMPERAMPER_t 260
# define _LPAREN_t 261
# define _RPAREN_t 262
# define _PLUS_EQUALS_t 263
# define _COLON_t 264
# define _SEMIC_t 265
# define _LANGLE_t 266
# define _LANGLE_EQUALS_t 267
# define _EQUALS_t 268
# define _RANGLE_t 269
# define _RANGLE_EQUALS_t 270
# define _QUESTION_EQUALS_t 271
# define _LBRACKET_t 272
# define _RBRACKET_t 273
# define ACTIONS_t 274
# define BIND_t 275
# define CASE_t 276
# define DEFAULT_t 277
# define ELSE_t 278
# define EXISTING_t 279
# define FOR_t 280
# define IF_t 281
# define IGNORE_t 282
# define IN_t 283
# define INCLUDE_t 284
# define LOCAL_t 285
# define MODULE_t 286
# define ON_t 287
# define PIECEMEAL_t 288
# define QUIETLY_t 289
# define RETURN_t 290
# define RULE_t 291
# define SWITCH_t 292
# define TOGETHER_t 293
# define UPDATED_t 294
# define WHILE_t 295
# define _LBRACE_t 296
# define _BAR_t 297
# define _BARBAR_t 298
# define _RBRACE_t 299
# define ARG 300
# define STRING 301

# line 98 "jamgram.y"
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

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE long
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256
static const yytabelem yyexca[] ={
-1, 0,
	291, 14,
	-2, 1,
-1, 1,
	0, -1,
	-2, 0,
-1, 3,
	291, 14,
	-2, 5,
-1, 4,
	291, 13,
	-2, 56,
-1, 5,
	299, 8,
	-2, 14,
-1, 7,
	264, 56,
	265, 56,
	272, 56,
	300, 56,
	-2, 58,
-1, 63,
	299, 8,
	-2, 14,
-1, 77,
	299, 8,
	-2, 14,
-1, 78,
	299, 8,
	-2, 14,
-1, 92,
	291, 14,
	-2, 8,
-1, 139,
	299, 8,
	-2, 14,
-1, 140,
	291, 14,
	-2, 8,
	};
# define YYNPROD 74
# define YYLAST 301

static const yytabelem yyact[]={

    18,   149,    17,   128,    79,    18,    18,    61,    10,    12,
   152,   150,     6,    36,    13,    16,   131,    18,     9,    17,
    11,    91,   130,    14,     5,    10,    12,   139,     7,     6,
     4,    13,    16,    42,    42,     9,   129,    11,   126,    53,
    14,     5,    65,    70,    71,     7,   113,    65,    70,    71,
    66,    67,    64,    68,    69,    66,    67,    64,    68,    69,
    65,    70,    71,    40,   134,    77,    62,    41,    66,    67,
    64,    68,    69,    18,    88,    36,    45,    85,    18,    97,
    74,    72,    73,    87,    86,    78,    72,    73,    84,    83,
   141,   137,   100,   119,   120,    81,    59,    31,    63,    72,
    73,    42,    30,    50,    31,    32,    42,   138,    95,    30,
    92,    33,    32,    60,    65,    70,    71,    55,    33,    65,
    54,    29,    66,    67,    64,    68,    69,    66,    67,    64,
    68,    69,    66,    67,   140,    68,    69,    56,   142,   117,
    39,     8,    98,    22,     8,    23,     8,     3,    26,    27,
    28,    24,     2,    15,    20,    19,    25,    46,    82,    34,
    89,    37,    52,    43,    35,    48,    51,    21,    99,   151,
   143,   118,    47,   116,    49,     1,     0,     0,    57,    58,
     0,     0,     0,     0,     0,     0,     0,     8,     0,    90,
     0,     0,     0,     0,    80,     0,     0,     0,     0,     0,
    93,     0,     0,     0,     8,    94,     0,   101,     0,    96,
     0,     0,     0,     0,     0,     0,     0,     0,     8,     8,
     0,   114,   115,     0,   112,     0,    38,     0,     0,     0,
     0,     0,   122,     8,     0,     0,   123,     0,     0,   121,
     0,    44,   127,     0,     0,     0,   124,   125,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     0,     0,
     0,     0,     0,   136,   132,     0,   133,    75,    76,   135,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     8,     8,     8,   146,   147,   144,     0,   145,     0,   148,
     0,   102,   103,   104,   105,   106,   107,   108,   109,   110,
   111 };

static const yytabelem yypact[]={

  -255, -1000, -1000,  -255, -1000,  -255, -1000, -1000,  -166, -1000,
  -210, -1000,  -194, -1000,  -194,  -215,  -267, -1000, -1000, -1000,
  -165,  -267,  -260, -1000, -1000,  -145,  -148,  -127, -1000, -1000,
 -1000, -1000, -1000,  -172,  -152,  -293, -1000,  -230,  -198,  -203,
  -194,  -194, -1000,  -231,  -211,  -296,  -272,  -205,  -266,  -155,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000,  -157,  -159, -1000,
 -1000,  -204,  -184,  -255,  -194,  -194,  -194,  -194,  -194,  -194,
  -194,  -194,  -194,  -194, -1000, -1000,  -216,  -255,  -255,  -122,
 -1000,  -182, -1000, -1000, -1000, -1000, -1000, -1000, -1000,  -179,
 -1000,  -267,  -255, -1000, -1000, -1000, -1000, -1000,  -261,  -184,
  -297,  -263,  -134,  -134, -1000, -1000, -1000, -1000,  -139,  -139,
  -144,  -144, -1000, -1000,  -277,  -283,  -272, -1000,  -232, -1000,
 -1000, -1000,  -199, -1000,  -158,  -269, -1000, -1000,  -130,  -188,
 -1000, -1000, -1000,  -124, -1000, -1000, -1000, -1000, -1000,  -255,
  -255,  -272, -1000,  -300, -1000, -1000,  -288, -1000, -1000, -1000,
 -1000,  -289, -1000 };

static const yytabelem yypgo[]={

     0,   175,   151,   143,   145,   147,   149,   174,   173,   148,
   153,   140,   150,   142,   226,   172,   171,   170,   169,   168,
   167,   165,   160,   158 };

static const yytabelem yyr1[]={

     0,     1,     1,     3,     3,     2,     2,     2,     4,     7,
     7,     8,     8,    10,    10,     5,     5,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,    17,
    18,     5,    12,    12,    12,    12,    14,    14,    14,    14,
    14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
    13,    13,    19,     9,     9,     6,    20,    20,    11,    21,
    11,    22,    22,    22,    15,    15,    23,    23,    23,    23,
    23,    23,    16,    16 };

static const yytabelem yyr2[]={

     0,     0,     3,     3,     3,     3,     5,    11,     1,     5,
     3,     7,     1,     3,     1,     7,     7,     7,     9,    13,
     7,    17,    11,    11,    11,    11,    15,    11,     7,     1,
     1,    19,     3,     3,     3,     5,     3,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     5,     7,
     1,     5,     9,     3,     7,     3,     1,     5,     3,     1,
     9,     5,     9,     9,     1,     5,     3,     3,     3,     3,
     3,     3,     1,     5 };

static const yytabelem yychk[]={

 -1000,    -1,    -2,    -5,   285,   296,   284,   300,   -11,   290,
   280,   292,   281,   286,   295,   -10,   287,   274,   272,    -2,
    -6,   -20,    -3,    -4,    -2,    -6,    -9,    -6,   -12,   287,
   268,   263,   271,   277,    -6,   -10,   285,    -6,   -14,   -11,
   257,   261,   300,    -6,   -14,   291,   -11,   -15,   -21,    -7,
   268,    -4,   -11,   299,   265,   265,   264,    -6,    -6,   268,
   265,   300,   296,   296,   268,   258,   266,   267,   269,   270,
   259,   260,   297,   298,   283,   -14,   -14,   296,   296,   300,
    -5,   300,   -23,   294,   293,   282,   289,   288,   279,   -22,
   -11,   287,   265,    -6,    -9,   265,   -12,   283,   -13,   -19,
   276,    -3,   -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,
   -14,   -14,    -6,   262,    -3,    -3,    -8,   261,   -16,   275,
   273,    -9,   -11,    -3,    -6,    -6,   299,   -13,   300,   299,
   299,   299,    -5,    -9,   296,    -6,   -11,   290,   265,   296,
   264,   278,   262,   -17,    -9,    -6,    -3,    -3,    -5,   301,
   299,   -18,   299 };

static const yytabelem yydef[]={

    -2,    -2,     2,    -2,    -2,    -2,    56,    -2,     0,    56,
    14,    56,     0,    56,     0,     0,     0,    64,    59,     6,
     8,    55,     0,     3,     4,     0,     0,    53,    56,    56,
    32,    33,    34,     0,     0,     0,    13,     0,     0,    36,
     0,     0,    58,     0,     0,     0,    14,     0,     0,     0,
    56,    10,    57,    15,    16,    17,    56,     0,     0,    35,
    20,     0,    50,    -2,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    56,    48,     0,    -2,    -2,    12,
    28,    72,    65,    66,    67,    68,    69,    70,    71,     0,
    56,     0,    -2,     9,    54,    18,    56,    56,     0,    50,
     0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    49,     0,     0,    14,    56,     0,    56,
    60,    61,     0,     7,     0,     0,    22,    51,     0,    23,
    24,    25,    27,     0,    29,    73,    56,    56,    19,    -2,
    -2,    14,    11,     0,    62,    63,     0,    52,    26,    30,
    21,     0,    31 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"_BANG_t",	257,
	"_BANG_EQUALS_t",	258,
	"_AMPER_t",	259,
	"_AMPERAMPER_t",	260,
	"_LPAREN_t",	261,
	"_RPAREN_t",	262,
	"_PLUS_EQUALS_t",	263,
	"_COLON_t",	264,
	"_SEMIC_t",	265,
	"_LANGLE_t",	266,
	"_LANGLE_EQUALS_t",	267,
	"_EQUALS_t",	268,
	"_RANGLE_t",	269,
	"_RANGLE_EQUALS_t",	270,
	"_QUESTION_EQUALS_t",	271,
	"_LBRACKET_t",	272,
	"_RBRACKET_t",	273,
	"ACTIONS_t",	274,
	"BIND_t",	275,
	"CASE_t",	276,
	"DEFAULT_t",	277,
	"ELSE_t",	278,
	"EXISTING_t",	279,
	"FOR_t",	280,
	"IF_t",	281,
	"IGNORE_t",	282,
	"IN_t",	283,
	"INCLUDE_t",	284,
	"LOCAL_t",	285,
	"MODULE_t",	286,
	"ON_t",	287,
	"PIECEMEAL_t",	288,
	"QUIETLY_t",	289,
	"RETURN_t",	290,
	"RULE_t",	291,
	"SWITCH_t",	292,
	"TOGETHER_t",	293,
	"UPDATED_t",	294,
	"WHILE_t",	295,
	"_LBRACE_t",	296,
	"_BAR_t",	297,
	"_BARBAR_t",	298,
	"_RBRACE_t",	299,
	"ARG",	300,
	"STRING",	301,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
      "run : /* empty */",
      "run : rules",
      "block : null",
      "block : rules",
      "rules : rule",
      "rules : rule rules",
      "rules : LOCAL_t list assign_list_opt _SEMIC_t block",
      "null : /* empty */",
      "assign_list_opt : _EQUALS_t list",
      "assign_list_opt : null",
      "arglist_opt : _LPAREN_t lol _RPAREN_t",
      "arglist_opt : /* empty */",
      "local_opt : LOCAL_t",
      "local_opt : /* empty */",
      "rule : _LBRACE_t block _RBRACE_t",
      "rule : INCLUDE_t list _SEMIC_t",
      "rule : ARG lol _SEMIC_t",
      "rule : arg assign list _SEMIC_t",
      "rule : arg ON_t list assign list _SEMIC_t",
      "rule : RETURN_t list _SEMIC_t",
      "rule : FOR_t local_opt ARG IN_t list _LBRACE_t block _RBRACE_t",
      "rule : SWITCH_t list _LBRACE_t cases _RBRACE_t",
      "rule : IF_t expr _LBRACE_t block _RBRACE_t",
      "rule : MODULE_t list _LBRACE_t block _RBRACE_t",
      "rule : WHILE_t expr _LBRACE_t block _RBRACE_t",
      "rule : IF_t expr _LBRACE_t block _RBRACE_t ELSE_t rule",
      "rule : local_opt RULE_t ARG arglist_opt rule",
      "rule : ON_t arg rule",
      "rule : ACTIONS_t eflags ARG bindlist _LBRACE_t",
      "rule : ACTIONS_t eflags ARG bindlist _LBRACE_t STRING",
      "rule : ACTIONS_t eflags ARG bindlist _LBRACE_t STRING _RBRACE_t",
      "assign : _EQUALS_t",
      "assign : _PLUS_EQUALS_t",
      "assign : _QUESTION_EQUALS_t",
      "assign : DEFAULT_t _EQUALS_t",
      "expr : arg",
      "expr : expr _EQUALS_t expr",
      "expr : expr _BANG_EQUALS_t expr",
      "expr : expr _LANGLE_t expr",
      "expr : expr _LANGLE_EQUALS_t expr",
      "expr : expr _RANGLE_t expr",
      "expr : expr _RANGLE_EQUALS_t expr",
      "expr : expr _AMPER_t expr",
      "expr : expr _AMPERAMPER_t expr",
      "expr : expr _BAR_t expr",
      "expr : expr _BARBAR_t expr",
      "expr : arg IN_t list",
      "expr : _BANG_t expr",
      "expr : _LPAREN_t expr _RPAREN_t",
      "cases : /* empty */",
      "cases : case cases",
      "case : CASE_t ARG _COLON_t block",
      "lol : list",
      "lol : list _COLON_t lol",
      "list : listp",
      "listp : /* empty */",
      "listp : listp arg",
      "arg : ARG",
      "arg : _LBRACKET_t",
      "arg : _LBRACKET_t func _RBRACKET_t",
      "func : arg lol",
      "func : ON_t arg arg lol",
      "func : ON_t arg RETURN_t list",
      "eflags : /* empty */",
      "eflags : eflags eflag",
      "eflag : UPDATED_t",
      "eflag : TOGETHER_t",
      "eflag : IGNORE_t",
      "eflag : QUIETLY_t",
      "eflag : PIECEMEAL_t",
      "eflag : EXISTING_t",
      "bindlist : /* empty */",
      "bindlist : BIND_t list",
};
#endif /* YYDEBUG */
/*
 * *****************************************************************
 * *                                                               *
 * *    Copyright Compaq Computer Corporation, 2000                *
 * *                                                               *
 * *   The software contained on this media  is  proprietary  to   *
 * *   and  embodies  the  confidential  technology  of  Compaq    *
 * *   Computer Corporation.  Possession, use,  duplication  or    *
 * *   dissemination of the software and media is authorized only  *
 * *   pursuant to a valid written license from Compaq Computer    *
 * *   Corporation.                                                *
 * *                                                               *
 * *   RESTRICTED RIGHTS LEGEND   Use, duplication, or disclosure  *
 * *   by the U.S. Government is subject to restrictions  as  set  *
 * *   forth in Subparagraph (c)(1)(ii)  of  DFARS  252.227-7013,  *
 * *   or  in  FAR 52.227-19, as applicable.                       *
 * *                                                               *
 * *****************************************************************
 */
/*
 * HISTORY
 */
/*
 * @(#)$RCSfile$ $Revision$ (DEC) $Date$
 */
/*
** Skeleton parser driver for yacc output
*/

/* external references for c++ and ANSI C
 * Define YY_NOPROTO to suppress the prototype declarations
 * GNUC and DECC define __STDC__ differently
 */
#ifdef __GNUC__
#if !__STDC__
#define YY_NOPROTO
#endif /* __STDC__ */
#elif !defined(__STDC__) &&  !defined (__cplusplus)
#define YY_NOPROTO
#endif /* __STDC__ */

/* Disable array out of bounds info messages */
#if defined (__DECC)
#pragma message disable (badsubscript,subscrbounds,unreach)
#endif

#ifndef YY_NOPROTO
#if defined (__cplusplus)
extern "C" {
extern void yyerror(char *); 
extern int yylex();
#else /* __cplusplus */
extern int yylex(void);
#endif /* __cplusplus */
#if defined (__cplusplus)
}
#endif /* __cplusplus */
#endif /* YY_NOPROTO */
/*
** yacc user known macros and defines
*/
#ifdef YYSPLIT
#   define YYERROR	return(-2)
#else
#   define YYERROR	goto yyerrlab
#endif

#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

#ifdef YYSPLIT
#   define YYSCODE { \
			extern int (*yyf[])(); \
			register int yyret; \
			if (yyf[yytmp]) \
			    if ((yyret=(*yyf[yytmp])()) == -2) \
				    goto yyerrlab; \
				else if (yyret>=0) return(yyret); \
		   }
#endif

/*
** local variables used by the parser
 * these should be static in order to support
 * multiple parsers in a single executable program. POSIX 1003.2-1993
 */
static YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
static int yys[ YYMAXDEPTH ];		/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static YYSTYPE *yypvt;			/* top of value stack for $vars */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

/*
** global variables used by the parser - renamed as a result of -p
*/
int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )	/* room on stack? */
		{
			yyerror( "yacc stack overflow" );
			YYABORT;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || 
               ( yy_n >= YYLAST )       || 
               (yyact[yy_n ] < 0))
               	goto yydefault;

		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register const int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/

		switch(yytmp){

case 2:	/* run : rules */
# line 142 "jamgram.y"
{ parse_save( yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 3:	/* block : null */
# line 153 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; } /*NOTREACHED*/ break;
case 4:	/* block : rules */
# line 155 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; } /*NOTREACHED*/ break;
case 5:	/* rules : rule */
# line 159 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; } /*NOTREACHED*/ break;
case 6:	/* rules : rule rules */
# line 161 "jamgram.y"
{ yyval.parse = prules( yypvt[-1].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 7:	/* rules : LOCAL_t list assign_list_opt _SEMIC_t block */
# line 163 "jamgram.y"
{ yyval.parse = plocal( yypvt[-3].parse, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 8:	/* null : Empty */
# line 167 "jamgram.y"
{ yyval.parse = pnull(); } /*NOTREACHED*/ break;
case 9:	/* assign_list_opt : _EQUALS_t list */
# line 171 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; yyval.number = ASSIGN_SET; } /*NOTREACHED*/ break;
case 10:	/* assign_list_opt : null */
# line 173 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; yyval.number = ASSIGN_APPEND; } /*NOTREACHED*/ break;
case 11:	/* arglist_opt : _LPAREN_t lol _RPAREN_t */
# line 177 "jamgram.y"
{ yyval.parse = yypvt[-1].parse; } /*NOTREACHED*/ break;
case 12:	/* arglist_opt : Empty */
# line 179 "jamgram.y"
{ yyval.parse = P0; } /*NOTREACHED*/ break;
case 13:	/* local_opt : LOCAL_t */
# line 183 "jamgram.y"
{ yyval.number = 1; } /*NOTREACHED*/ break;
case 14:	/* local_opt : Empty */
# line 185 "jamgram.y"
{ yyval.number = 0; } /*NOTREACHED*/ break;
case 15:	/* rule : _LBRACE_t block _RBRACE_t */
# line 189 "jamgram.y"
{ yyval.parse = yypvt[-1].parse; } /*NOTREACHED*/ break;
case 16:	/* rule : INCLUDE_t list _SEMIC_t */
# line 191 "jamgram.y"
{ yyval.parse = pincl( yypvt[-1].parse ); } /*NOTREACHED*/ break;
case 17:	/* rule : ARG lol _SEMIC_t */
# line 193 "jamgram.y"
{ yyval.parse = prule( yypvt[-2].string, yypvt[-1].parse ); } /*NOTREACHED*/ break;
case 18:	/* rule : arg assign list _SEMIC_t */
# line 195 "jamgram.y"
{ yyval.parse = pset( yypvt[-3].parse, yypvt[-1].parse, yypvt[-2].number ); } /*NOTREACHED*/ break;
case 19:	/* rule : arg ON_t list assign list _SEMIC_t */
# line 197 "jamgram.y"
{ yyval.parse = pset1( yypvt[-5].parse, yypvt[-3].parse, yypvt[-1].parse, yypvt[-2].number ); } /*NOTREACHED*/ break;
case 20:	/* rule : RETURN_t list _SEMIC_t */
# line 199 "jamgram.y"
{ yyval.parse = yypvt[-1].parse; } /*NOTREACHED*/ break;
case 21:	/* rule : FOR_t local_opt ARG IN_t list _LBRACE_t block _RBRACE_t */
# line 201 "jamgram.y"
{ yyval.parse = pfor( yypvt[-5].string, yypvt[-3].parse, yypvt[-1].parse, yypvt[-6].number ); } /*NOTREACHED*/ break;
case 22:	/* rule : SWITCH_t list _LBRACE_t cases _RBRACE_t */
# line 203 "jamgram.y"
{ yyval.parse = pswitch( yypvt[-3].parse, yypvt[-1].parse ); } /*NOTREACHED*/ break;
case 23:	/* rule : IF_t expr _LBRACE_t block _RBRACE_t */
# line 205 "jamgram.y"
{ yyval.parse = pif( yypvt[-3].parse, yypvt[-1].parse, pnull() ); } /*NOTREACHED*/ break;
case 24:	/* rule : MODULE_t list _LBRACE_t block _RBRACE_t */
# line 207 "jamgram.y"
{ yyval.parse = pmodule( yypvt[-3].parse, yypvt[-1].parse ); } /*NOTREACHED*/ break;
case 25:	/* rule : WHILE_t expr _LBRACE_t block _RBRACE_t */
# line 209 "jamgram.y"
{ yyval.parse = pwhile( yypvt[-3].parse, yypvt[-1].parse ); } /*NOTREACHED*/ break;
case 26:	/* rule : IF_t expr _LBRACE_t block _RBRACE_t ELSE_t rule */
# line 211 "jamgram.y"
{ yyval.parse = pif( yypvt[-5].parse, yypvt[-3].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 27:	/* rule : local_opt RULE_t ARG arglist_opt rule */
# line 213 "jamgram.y"
{ yyval.parse = psetc( yypvt[-2].string, yypvt[-0].parse, yypvt[-1].parse, yypvt[-4].number ); } /*NOTREACHED*/ break;
case 28:	/* rule : ON_t arg rule */
# line 215 "jamgram.y"
{ yyval.parse = pon( yypvt[-1].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 29:	/* rule : ACTIONS_t eflags ARG bindlist _LBRACE_t */
# line 217 "jamgram.y"
{ yymode( SCAN_STRING ); } /*NOTREACHED*/ break;
case 30:	/* rule : ACTIONS_t eflags ARG bindlist _LBRACE_t STRING */
# line 219 "jamgram.y"
{ yymode( SCAN_NORMAL ); } /*NOTREACHED*/ break;
case 31:	/* rule : ACTIONS_t eflags ARG bindlist _LBRACE_t STRING _RBRACE_t */
# line 221 "jamgram.y"
{ yyval.parse = psete( yypvt[-6].string,yypvt[-5].parse,yypvt[-2].string,yypvt[-7].number ); } /*NOTREACHED*/ break;
case 32:	/* assign : _EQUALS_t */
# line 229 "jamgram.y"
{ yyval.number = ASSIGN_SET; } /*NOTREACHED*/ break;
case 33:	/* assign : _PLUS_EQUALS_t */
# line 231 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; } /*NOTREACHED*/ break;
case 34:	/* assign : _QUESTION_EQUALS_t */
# line 233 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; } /*NOTREACHED*/ break;
case 35:	/* assign : DEFAULT_t _EQUALS_t */
# line 235 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; } /*NOTREACHED*/ break;
case 36:	/* expr : arg */
# line 242 "jamgram.y"
{ yyval.parse = peval( EXPR_EXISTS, yypvt[-0].parse, pnull() ); } /*NOTREACHED*/ break;
case 37:	/* expr : expr _EQUALS_t expr */
# line 244 "jamgram.y"
{ yyval.parse = peval( EXPR_EQUALS, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 38:	/* expr : expr _BANG_EQUALS_t expr */
# line 246 "jamgram.y"
{ yyval.parse = peval( EXPR_NOTEQ, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 39:	/* expr : expr _LANGLE_t expr */
# line 248 "jamgram.y"
{ yyval.parse = peval( EXPR_LESS, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 40:	/* expr : expr _LANGLE_EQUALS_t expr */
# line 250 "jamgram.y"
{ yyval.parse = peval( EXPR_LESSEQ, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 41:	/* expr : expr _RANGLE_t expr */
# line 252 "jamgram.y"
{ yyval.parse = peval( EXPR_MORE, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 42:	/* expr : expr _RANGLE_EQUALS_t expr */
# line 254 "jamgram.y"
{ yyval.parse = peval( EXPR_MOREEQ, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 43:	/* expr : expr _AMPER_t expr */
# line 256 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 44:	/* expr : expr _AMPERAMPER_t expr */
# line 258 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 45:	/* expr : expr _BAR_t expr */
# line 260 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 46:	/* expr : expr _BARBAR_t expr */
# line 262 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 47:	/* expr : arg IN_t list */
# line 264 "jamgram.y"
{ yyval.parse = peval( EXPR_IN, yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 48:	/* expr : _BANG_t expr */
# line 266 "jamgram.y"
{ yyval.parse = peval( EXPR_NOT, yypvt[-0].parse, pnull() ); } /*NOTREACHED*/ break;
case 49:	/* expr : _LPAREN_t expr _RPAREN_t */
# line 268 "jamgram.y"
{ yyval.parse = yypvt[-1].parse; } /*NOTREACHED*/ break;
case 50:	/* cases : Empty */
# line 279 "jamgram.y"
{ yyval.parse = P0; } /*NOTREACHED*/ break;
case 51:	/* cases : case cases */
# line 281 "jamgram.y"
{ yyval.parse = pnode( yypvt[-1].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 52:	/* case : CASE_t ARG _COLON_t block */
# line 285 "jamgram.y"
{ yyval.parse = psnode( yypvt[-2].string, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 53:	/* lol : list */
# line 294 "jamgram.y"
{ yyval.parse = pnode( P0, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 54:	/* lol : list _COLON_t lol */
# line 296 "jamgram.y"
{ yyval.parse = pnode( yypvt[-0].parse, yypvt[-2].parse ); } /*NOTREACHED*/ break;
case 55:	/* list : listp */
# line 306 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; yymode( SCAN_NORMAL ); } /*NOTREACHED*/ break;
case 56:	/* listp : Empty */
# line 310 "jamgram.y"
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); } /*NOTREACHED*/ break;
case 57:	/* listp : listp arg */
# line 312 "jamgram.y"
{ yyval.parse = pappend( yypvt[-1].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 58:	/* arg : ARG */
# line 316 "jamgram.y"
{ yyval.parse = plist( yypvt[-0].string ); } /*NOTREACHED*/ break;
case 59:	/* arg : _LBRACKET_t */
# line 317 "jamgram.y"
{ yymode( SCAN_NORMAL ); } /*NOTREACHED*/ break;
case 60:	/* arg : _LBRACKET_t func _RBRACKET_t */
# line 318 "jamgram.y"
{ yyval.parse = yypvt[-1].parse; } /*NOTREACHED*/ break;
case 61:	/* func : arg lol */
# line 327 "jamgram.y"
{ yyval.parse = prule( yypvt[-1].string, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 62:	/* func : ON_t arg arg lol */
# line 329 "jamgram.y"
{ yyval.parse = pon( yypvt[-2].parse, prule( yypvt[-1].string, yypvt[-0].parse ) ); } /*NOTREACHED*/ break;
case 63:	/* func : ON_t arg RETURN_t list */
# line 331 "jamgram.y"
{ yyval.parse = pon( yypvt[-2].parse, yypvt[-0].parse ); } /*NOTREACHED*/ break;
case 64:	/* eflags : Empty */
# line 341 "jamgram.y"
{ yyval.number = 0; } /*NOTREACHED*/ break;
case 65:	/* eflags : eflags eflag */
# line 343 "jamgram.y"
{ yyval.number = yypvt[-1].number | yypvt[-0].number; } /*NOTREACHED*/ break;
case 66:	/* eflag : UPDATED_t */
# line 347 "jamgram.y"
{ yyval.number = EXEC_UPDATED; } /*NOTREACHED*/ break;
case 67:	/* eflag : TOGETHER_t */
# line 349 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; } /*NOTREACHED*/ break;
case 68:	/* eflag : IGNORE_t */
# line 351 "jamgram.y"
{ yyval.number = EXEC_IGNORE; } /*NOTREACHED*/ break;
case 69:	/* eflag : QUIETLY_t */
# line 353 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; } /*NOTREACHED*/ break;
case 70:	/* eflag : PIECEMEAL_t */
# line 355 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; } /*NOTREACHED*/ break;
case 71:	/* eflag : EXISTING_t */
# line 357 "jamgram.y"
{ yyval.number = EXEC_EXISTING; } /*NOTREACHED*/ break;
case 72:	/* bindlist : Empty */
# line 366 "jamgram.y"
{ yyval.parse = pnull(); } /*NOTREACHED*/ break;
case 73:	/* bindlist : BIND_t list */
# line 368 "jamgram.y"
{ yyval.parse = yypvt[-0].parse; } /*NOTREACHED*/ break;
}


	goto yystack;		/* reset registers in driver code */
}
