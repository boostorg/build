#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 98 "jamgram.y"
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

#line 55 "y.tab.c"
#define YYERRCODE 256
#define _BANG_t 257
#define _BANG_EQUALS_t 258
#define _AMPER_t 259
#define _AMPERAMPER_t 260
#define _LPAREN_t 261
#define _RPAREN_t 262
#define _PLUS_EQUALS_t 263
#define _COLON_t 264
#define _SEMIC_t 265
#define _LANGLE_t 266
#define _LANGLE_EQUALS_t 267
#define _EQUALS_t 268
#define _RANGLE_t 269
#define _RANGLE_EQUALS_t 270
#define _QUESTION_EQUALS_t 271
#define _LBRACKET_t 272
#define _RBRACKET_t 273
#define ACTIONS_t 274
#define BIND_t 275
#define CASE_t 276
#define DEFAULT_t 277
#define ELSE_t 278
#define EXISTING_t 279
#define FOR_t 280
#define IF_t 281
#define IGNORE_t 282
#define IN_t 283
#define INCLUDE_t 284
#define LOCAL_t 285
#define MODULE_t 286
#define ON_t 287
#define PIECEMEAL_t 288
#define QUIETLY_t 289
#define RETURN_t 290
#define RULE_t 291
#define SWITCH_t 292
#define TOGETHER_t 293
#define UPDATED_t 294
#define WHILE_t 295
#define _LBRACE_t 296
#define _BAR_t 297
#define _BARBAR_t 298
#define _RBRACE_t 299
#define ARG 300
#define STRING 301
const short yylhs[] = {                                        -1,
    0,    0,    2,    2,    1,    1,    1,    3,    6,    6,
    7,    7,    9,    9,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,   16,   17,
    4,   11,   11,   11,   11,   13,   13,   13,   13,   13,
   13,   13,   13,   13,   13,   13,   13,   13,   13,   12,
   12,   18,    8,    8,    5,   19,   19,   10,   21,   10,
   20,   20,   20,   14,   14,   22,   22,   22,   22,   22,
   22,   15,   15,
};
const short yylen[] = {                                         2,
    0,    1,    1,    1,    1,    2,    5,    0,    2,    1,
    3,    0,    1,    0,    3,    3,    3,    4,    6,    3,
    8,    5,    5,    5,    5,    7,    5,    3,    0,    0,
    9,    1,    1,    1,    2,    1,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    2,    3,    0,
    2,    4,    1,    3,    1,    0,    2,    1,    0,    4,
    2,    4,    4,    0,    2,    1,    1,    1,    1,    1,
    1,    0,    2,
};
const short yydefred[] = {                                      0,
   59,   64,    0,    0,   56,    0,   56,    0,   56,   56,
    0,    0,    0,    0,    2,    0,    0,    0,    0,    0,
   13,    0,    0,    0,   58,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    4,    0,    3,    0,    0,
    6,    0,   33,   32,   34,    0,   56,   56,    0,   56,
    0,   71,   68,   70,   69,   67,   66,    0,   65,    0,
   48,    0,   56,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   16,   57,   56,   10,    0,    0,
   28,   20,    0,    0,   15,   56,   17,    0,   35,    0,
    0,    0,   61,   60,   56,    0,   56,   49,   47,    0,
    0,    0,   39,   40,    0,   41,   42,    0,    0,    0,
    9,    0,    0,    0,    0,    0,    0,   54,   56,    0,
   56,   18,   56,   56,   73,   29,    0,    0,    7,   24,
    0,   22,   51,   25,    0,   27,    0,   63,   62,    0,
    0,    0,    0,   11,   19,   30,    0,   26,   52,    0,
   21,   31,
};
const short yydgoto[] = {                                      14,
   36,   37,   38,   16,   39,   79,  120,   40,   17,   26,
   48,  115,   27,   20,   96,  140,  150,  116,   29,   51,
   19,   59,
};
const short yysindex[] = {                                     57,
    0,    0, -277, -235,    0,    0,    0, -248,    0,    0,
 -235,   57,    0,    0,    0,   57, -264, -145, -208,  -28,
    0, -271, -235, -235,    0, -242, -220, -210, -248, -238,
 -240,   74, -203, -233,  -80,    0, -231,    0, -198, -192,
    0, -219,    0,    0,    0, -184,    0,    0, -248,    0,
 -188,    0,    0,    0,    0,    0,    0, -185,    0, -196,
    0,  -60,    0, -235, -235, -235, -235, -235, -235, -235,
 -235,   57, -235, -235,    0,    0,    0,    0, -156,   57,
    0,    0, -164,   57,    0,    0,    0, -146,    0, -143,
 -148, -267,    0,    0,    0, -177,    0,    0,    0,  106,
   41,   41,    0,    0,  106,    0,    0, -178,  -35,  -35,
    0,   57, -168, -167, -159, -164, -158,    0,    0,   74,
    0,    0,    0,    0,    0,    0, -142, -118,    0,    0,
 -129,    0,    0,    0,  -91,    0,  -72,    0,    0, -100,
   57,   74,   57,    0,    0,    0,  -96,    0,    0,  -95,
    0,    0,
};
const short yyrindex[] = {                                      7,
    0,    0,  -86,    0,    0, -161,    0,    0,    0,    0,
    0, -274,  -24,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -85,    0,    0, -101,  -50,
    0,  -70,    0,    0,    0,    0,    0,    0, -149,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -53,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -274,    0,    0,    0,    0,    0,    0,    0, -274,
    0,    0,  -94, -274,    0,    0,    0,   32,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -201,
  -68,  -40,    0,    0, -160,    0,    0,    0, -253, -226,
    0,    4,    0,    0,    0,  -94,    0,    0,    0,  -70,
    0,    0,    0,    0,    0,    0,    0,    1,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -274,  -70,  -49,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
const short yygindex[] = {                                      0,
   19,  -52,  196,  -32,    6,    0,    0,  -44,  233,    2,
  155,  130,   82,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 376
const short yytable[] = {                                      81,
   23,   18,    5,    8,    1,   93,    1,   21,   45,   32,
   28,   30,   31,   18,   33,   34,   14,   18,   15,  108,
   50,   23,  123,    1,    8,   24,   42,  113,   60,   77,
   76,  117,   25,   18,   41,   46,    1,   64,   65,   66,
   63,  118,   45,   45,   45,   67,   68,   69,   70,   71,
   92,   25,   90,   91,   75,   80,   38,   38,   38,  129,
   38,   82,   83,    1,   25,   86,   38,   85,   99,   46,
   46,   46,   87,   18,  135,   72,   73,   74,   49,  139,
   88,   18,  111,   89,   94,   18,   97,  136,  147,   95,
  149,   25,   35,  124,   38,   38,   38,   37,   37,   37,
  125,   37,  127,   56,   61,   62,   56,   37,  112,  148,
   56,  114,   53,   18,  119,   53,  122,   43,  126,   43,
  128,   18,   44,   53,   44,   45,  137,   45,  138,   13,
  130,   46,  131,   46,  143,   37,   37,   37,   56,  132,
  134,   47,   18,   18,   18,  100,  101,  102,  103,  104,
  105,  106,  107,  141,  109,  110,   55,   55,   55,  142,
   55,   55,   55,   55,   55,   55,   55,   55,   55,   55,
  144,   55,   36,   36,   36,   55,   36,   64,   65,   66,
   36,   36,   36,   36,   36,   67,   68,   69,   70,   71,
   43,   43,  145,   43,   55,   55,   55,   64,   65,   66,
  146,   98,  151,  152,   50,   67,   68,   69,   70,   71,
   36,   36,   36,   14,    8,   84,   73,   74,   44,   44,
   14,   44,   64,   65,   66,   78,    8,   43,   43,   43,
   67,   68,   69,   70,   71,   22,   73,   74,   58,   56,
   56,   14,   72,   58,  121,  133,   58,   56,    0,    8,
   52,    0,   58,   53,    0,   44,   44,   44,    0,   54,
   55,    0,   58,    0,   56,   57,    0,    0,    0,    0,
    0,   58,   23,    0,   23,   56,   23,    0,    5,    8,
   23,   23,    0,    0,   23,   23,   23,   23,    0,    0,
   23,   23,   23,   14,   14,   23,   23,   14,   64,   23,
   23,    5,    8,   12,    0,   12,   67,   68,   69,   70,
   71,   12,   12,    0,    0,   12,   12,   12,   12,    0,
    0,   12,   12,   12,    0,    0,   12,   12,    1,    0,
    2,   12,    0,    0,    0,    0,    3,    4,    0,    0,
    5,    6,    7,    8,    0,    1,    9,    2,   10,    0,
    0,   11,   12,    3,    4,    0,   13,    5,   21,    7,
    8,    0,    0,    9,    0,   10,    0,    0,   11,   12,
    0,   67,   68,   13,   70,   71,
};
const short yycheck[] = {                                      32,
    0,    0,    0,    0,  272,   50,    0,  285,  262,    8,
    5,    6,    7,   12,    9,   10,  291,   16,    0,   72,
   19,  257,  290,  272,  299,  261,  291,   80,  300,  268,
   29,   84,  300,   32,   16,  262,  272,  258,  259,  260,
  283,   86,  296,  297,  298,  266,  267,  268,  269,  270,
   49,  300,   47,   48,  265,  296,  258,  259,  260,  112,
  262,  265,  296,  272,  300,  264,  268,  299,   63,  296,
  297,  298,  265,   72,  119,  296,  297,  298,  287,  124,
  300,   80,   77,  268,  273,   84,  283,  120,  141,  275,
  143,  300,   11,   92,  296,  297,  298,  258,  259,  260,
   95,  262,   97,  265,   23,   24,  268,  268,  265,  142,
  272,  276,  262,  112,  261,  265,  265,  263,  296,  263,
  299,  120,  268,  273,  268,  271,  121,  271,  123,  291,
  299,  277,  300,  277,  264,  296,  297,  298,  300,  299,
  299,  287,  141,  142,  143,   64,   65,   66,   67,   68,
   69,   70,   71,  296,   73,   74,  258,  259,  260,  278,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  262,  273,  258,  259,  260,  277,  262,  258,  259,  260,
  266,  267,  268,  269,  270,  266,  267,  268,  269,  270,
  259,  260,  265,  262,  296,  297,  298,  258,  259,  260,
  301,  262,  299,  299,  299,  266,  267,  268,  269,  270,
  296,  297,  298,  300,  265,  296,  297,  298,  259,  260,
  291,  262,  258,  259,  260,   30,  276,  296,  297,  298,
  266,  267,  268,  269,  270,    3,  297,  298,  263,  264,
  265,  291,  296,  268,   90,  116,  271,  272,   -1,  299,
  279,   -1,  277,  282,   -1,  296,  297,  298,   -1,  288,
  289,   -1,  287,   -1,  293,  294,   -1,   -1,   -1,   -1,
   -1,  300,  272,   -1,  274,  300,  276,   -1,  276,  276,
  280,  281,   -1,   -1,  284,  285,  286,  287,   -1,   -1,
  290,  291,  292,  291,  291,  295,  296,  291,  258,  299,
  300,  299,  299,  272,   -1,  274,  266,  267,  268,  269,
  270,  280,  281,   -1,   -1,  284,  285,  286,  287,   -1,
   -1,  290,  291,  292,   -1,   -1,  295,  296,  272,   -1,
  274,  300,   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,
  284,  285,  286,  287,   -1,  272,  290,  274,  292,   -1,
   -1,  295,  296,  280,  281,   -1,  300,  284,  285,  286,
  287,   -1,   -1,  290,   -1,  292,   -1,   -1,  295,  296,
   -1,  266,  267,  300,  269,  270,
};
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 301
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"_BANG_t","_BANG_EQUALS_t",
"_AMPER_t","_AMPERAMPER_t","_LPAREN_t","_RPAREN_t","_PLUS_EQUALS_t","_COLON_t",
"_SEMIC_t","_LANGLE_t","_LANGLE_EQUALS_t","_EQUALS_t","_RANGLE_t",
"_RANGLE_EQUALS_t","_QUESTION_EQUALS_t","_LBRACKET_t","_RBRACKET_t","ACTIONS_t",
"BIND_t","CASE_t","DEFAULT_t","ELSE_t","EXISTING_t","FOR_t","IF_t","IGNORE_t",
"IN_t","INCLUDE_t","LOCAL_t","MODULE_t","ON_t","PIECEMEAL_t","QUIETLY_t",
"RETURN_t","RULE_t","SWITCH_t","TOGETHER_t","UPDATED_t","WHILE_t","_LBRACE_t",
"_BAR_t","_BARBAR_t","_RBRACE_t","ARG","STRING",
};
const char * const yyrule[] = {
"$accept : run",
"run :",
"run : rules",
"block : null",
"block : rules",
"rules : rule",
"rules : rule rules",
"rules : LOCAL_t list assign_list_opt _SEMIC_t block",
"null :",
"assign_list_opt : _EQUALS_t list",
"assign_list_opt : null",
"arglist_opt : _LPAREN_t lol _RPAREN_t",
"arglist_opt :",
"local_opt : LOCAL_t",
"local_opt :",
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
"$$1 :",
"$$2 :",
"rule : ACTIONS_t eflags ARG bindlist _LBRACE_t $$1 STRING $$2 _RBRACE_t",
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
"cases :",
"cases : case cases",
"case : CASE_t ARG _COLON_t block",
"lol : list",
"lol : list _COLON_t lol",
"list : listp",
"listp :",
"listp : listp arg",
"arg : ARG",
"$$3 :",
"arg : _LBRACKET_t $$3 func _RBRACKET_t",
"func : arg lol",
"func : ON_t arg arg lol",
"func : ON_t arg RETURN_t list",
"eflags :",
"eflags : eflags eflag",
"eflag : UPDATED_t",
"eflag : TOGETHER_t",
"eflag : IGNORE_t",
"eflag : QUIETLY_t",
"eflag : PIECEMEAL_t",
"eflag : EXISTING_t",
"bindlist :",
"bindlist : BIND_t list",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
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
#line 878 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
