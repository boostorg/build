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
#line 99 "jamgram.y"
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
# define peval( c,l,r )	parse_make( compile_eval,l,r,P0,S0,S0,c )
# define pshortcircuiteval( c,l,r )	parse_make( compile_eval,l,P0,r,S0,S0,c )
# define pfor( s,l,r,x )    	parse_make( compile_foreach,l,r,P0,s,S0,x )
# define pif( l,r,t )	  	parse_make( compile_if,l,r,t,S0,S0,0 )
# define pwhile( l,r )	  	parse_make( compile_while,l,r,P0,S0,S0,0 )
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

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

#line 53 "y.tab.c"
#define YYERRCODE 256
#define _BANG 257
#define _BANG_EQUALS 258
#define _AMPER 259
#define _AMPERAMPER 260
#define _LPAREN 261
#define _RPAREN 262
#define _PLUS_EQUALS 263
#define _COLON 264
#define _SEMIC 265
#define _LANGLE 266
#define _LANGLE_EQUALS 267
#define _EQUALS 268
#define _RANGLE 269
#define _RANGLE_EQUALS 270
#define _QUESTION_EQUALS 271
#define _LBRACKET 272
#define _RBRACKET 273
#define ACTIONS 274
#define BIND 275
#define CASE 276
#define DEFAULT 277
#define ELSE 278
#define EXISTING 279
#define FOR 280
#define IF 281
#define IGNORE 282
#define IN 283
#define INCLUDE 284
#define LOCAL 285
#define MODULE 286
#define ON 287
#define PIECEMEAL 288
#define QUIETLY 289
#define RETURN 290
#define RULE 291
#define SWITCH 292
#define TOGETHER 293
#define UPDATED 294
#define WHILE 295
#define _LBRACE 296
#define _BAR 297
#define _BARBAR 298
#define _RBRACE 299
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
   48,    0,    0,    0,    0,    0,    0,    0,   56,    0,
    0,    0,    0,    0,   16,   57,   56,   10,    0,    0,
   28,   20,    0,    0,   15,   56,   17,    0,   35,    0,
    0,    0,   61,   60,   56,    0,   56,   49,   38,   39,
   40,   37,   41,   42,   47,   43,   44,    0,    0,    0,
    9,    0,    0,    0,    0,    0,    0,   54,   56,    0,
   56,   18,   56,   56,   73,   29,    0,    0,    7,   24,
    0,   22,   51,   25,    0,   27,    0,   63,   62,    0,
    0,    0,    0,   11,   19,   30,    0,   26,   52,    0,
   21,   31,
};
const short yydgoto[] = {                                      14,
   36,   37,   38,   16,   39,   79,  120,   40,   17,   18,
   48,  115,   27,   20,   96,  140,  150,  116,   29,   51,
   19,   59,
};
const short yysindex[] = {                                    -50,
    0,    0, -254, -238,    0,    0,    0, -250,    0,    0,
 -238,  -50,    0,    0,    0,  -50, -258, -144, -261,  -24,
    0, -270, -238, -238,    0, -129, -202, -214, -250, -212,
 -242,  -33, -210, -237, -199,    0, -236,    0, -198, -196,
    0, -230,    0,    0,    0, -193,    0,    0, -250,    0,
 -201,    0,    0,    0,    0,    0,    0, -188,    0, -194,
    0, -217, -250, -250, -250, -250, -250, -250,    0, -238,
 -238,  -50, -238, -238,    0,    0,    0,    0, -174,  -50,
    0,    0, -175,  -50,    0,    0,    0, -169,    0, -104,
 -162, -265,    0,    0,    0, -192,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -183, -232, -232,
    0,  -50, -182, -185, -181, -175, -179,    0,    0,  -33,
    0,    0,    0,    0,    0,    0, -173, -157,    0,    0,
 -142,    0,    0,    0, -137,    0, -135,    0,    0, -167,
  -50,  -33,  -50,    0,    0,    0, -164,    0,    0, -163,
    0,    0,
};
const short yyrindex[] = {                                      7,
    0,    0, -168,    0,    0, -259,    0,    0,    0,    0,
    0, -262,  -87,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -213,    0,    0, -102, -123,
    0, -147,    0,    0,    0,    0,    0,    0,  -82,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -262,    0,    0,    0,    0,    0,    0,    0, -262,
    0,    0, -134, -262,    0,    0,    0,  -75,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -189, -184,
    0,    4,    0,    0,    0, -134,    0,    0,    0, -147,
    0,    0,    0,    0,    0,    0,    0,    1,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -262, -147, -255,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
const short yygindex[] = {                                      0,
   24,  -64,  138,  -32,    5,    0,    0,  -48,  167,   82,
   84,   56,   -6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 303
const short yytable[] = {                                      81,
   23,   93,    5,    8,   35,   56,    1,  108,   56,   28,
   30,   31,   56,   33,   34,  113,   61,   62,   23,  117,
    8,    1,   24,   15,  123,   49,   70,   71,   14,   60,
   21,   13,   42,    1,  124,   14,    8,  118,   50,   41,
   56,   70,   71,    8,   98,   36,   36,  129,   36,   25,
   75,   90,   91,   80,   82,   77,   70,   71,   83,   70,
   71,   25,   85,  106,  107,   86,  109,  110,   87,   88,
  135,   94,   45,  105,   89,  139,  147,   46,  149,   73,
   74,  111,   36,   36,   36,   26,   95,  136,   97,   32,
  112,  119,   26,   72,   73,   74,   84,   73,   74,  125,
  114,  127,  122,  126,   26,   26,   45,   45,   45,  148,
   76,   46,   46,   46,  131,  128,  130,  132,   43,  134,
  142,  143,  141,   44,  144,  137,   45,  138,   63,  145,
   92,   14,   46,  146,  151,  152,   64,   65,   66,   67,
   68,    8,   47,   14,   99,  100,  101,  102,  103,  104,
   72,   26,   26,   69,   26,   26,   55,   55,   43,   55,
   55,   55,   55,   44,   50,   55,   45,   78,   55,   22,
   55,  133,   46,  121,   55,   58,   56,   56,    0,   53,
   58,    0,   53,   58,   56,    0,    0,    0,    0,   58,
   53,    0,    0,   55,   55,   55,   12,    0,   12,   58,
    0,    0,    0,    0,   12,   12,    0,    0,   12,   12,
   12,   12,   56,    0,   12,   12,   12,    0,    0,   12,
   12,    1,    0,    2,   12,    0,    0,    0,    0,    3,
    4,    0,    0,    5,    6,    7,    8,    0,    1,    9,
    2,   10,    0,    0,   11,   12,    3,    4,    0,   13,
    5,   21,    7,    8,   52,    0,    9,   53,   10,    0,
    0,   11,   12,   54,   55,    0,   13,    0,   56,   57,
    0,    0,   23,    0,   23,   58,   23,    0,    5,    8,
   23,   23,    0,    0,   23,   23,   23,   23,    0,    0,
   23,   23,   23,   14,   14,   23,   23,   14,    0,   23,
   23,    5,    8,
};
const short yycheck[] = {                                      32,
    0,   50,    0,    0,   11,  265,    0,   72,  268,    5,
    6,    7,  272,    9,   10,   80,   23,   24,  257,   84,
  276,  272,  261,    0,  290,  287,  259,  260,  291,  300,
  285,  291,  291,  272,  300,  291,  299,   86,  300,   16,
  300,  259,  260,  299,  262,  259,  260,  112,  262,  300,
  265,   47,   48,  296,  265,  268,  259,  260,  296,  259,
  260,  300,  299,   70,   71,  264,   73,   74,  265,  300,
  119,  273,  262,   69,  268,  124,  141,  262,  143,  297,
  298,   77,  296,  297,  298,    4,  275,  120,  283,    8,
  265,  261,   11,  296,  297,  298,  296,  297,  298,   95,
  276,   97,  265,  296,   23,   24,  296,  297,  298,  142,
   29,  296,  297,  298,  300,  299,  299,  299,  263,  299,
  278,  264,  296,  268,  262,  121,  271,  123,  258,  265,
   49,  300,  277,  301,  299,  299,  266,  267,  268,  269,
  270,  265,  287,  291,   63,   64,   65,   66,   67,   68,
  296,   70,   71,  283,   73,   74,  259,  260,  263,  262,
  263,  264,  265,  268,  299,  268,  271,   30,  271,    3,
  273,  116,  277,   90,  277,  263,  264,  265,   -1,  262,
  268,   -1,  265,  271,  272,   -1,   -1,   -1,   -1,  277,
  273,   -1,   -1,  296,  297,  298,  272,   -1,  274,  287,
   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,  284,  285,
  286,  287,  300,   -1,  290,  291,  292,   -1,   -1,  295,
  296,  272,   -1,  274,  300,   -1,   -1,   -1,   -1,  280,
  281,   -1,   -1,  284,  285,  286,  287,   -1,  272,  290,
  274,  292,   -1,   -1,  295,  296,  280,  281,   -1,  300,
  284,  285,  286,  287,  279,   -1,  290,  282,  292,   -1,
   -1,  295,  296,  288,  289,   -1,  300,   -1,  293,  294,
   -1,   -1,  272,   -1,  274,  300,  276,   -1,  276,  276,
  280,  281,   -1,   -1,  284,  285,  286,  287,   -1,   -1,
  290,  291,  292,  291,  291,  295,  296,  291,   -1,  299,
  300,  299,  299,
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
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"_BANG","_BANG_EQUALS","_AMPER",
"_AMPERAMPER","_LPAREN","_RPAREN","_PLUS_EQUALS","_COLON","_SEMIC","_LANGLE",
"_LANGLE_EQUALS","_EQUALS","_RANGLE","_RANGLE_EQUALS","_QUESTION_EQUALS",
"_LBRACKET","_RBRACKET","ACTIONS","BIND","CASE","DEFAULT","ELSE","EXISTING",
"FOR","IF","IGNORE","IN","INCLUDE","LOCAL","MODULE","ON","PIECEMEAL","QUIETLY",
"RETURN","RULE","SWITCH","TOGETHER","UPDATED","WHILE","_LBRACE","_BAR",
"_BARBAR","_RBRACE","ARG","STRING",
};
const char * const yyrule[] = {
"$accept : run",
"run :",
"run : rules",
"block : null",
"block : rules",
"rules : rule",
"rules : rule rules",
"rules : LOCAL list assign_list_opt _SEMIC block",
"null :",
"assign_list_opt : _EQUALS list",
"assign_list_opt : null",
"arglist_opt : _LPAREN lol _RPAREN",
"arglist_opt :",
"local_opt : LOCAL",
"local_opt :",
"rule : _LBRACE block _RBRACE",
"rule : INCLUDE list _SEMIC",
"rule : ARG lol _SEMIC",
"rule : arg assign list _SEMIC",
"rule : arg ON list assign list _SEMIC",
"rule : RETURN list _SEMIC",
"rule : FOR local_opt ARG IN list _LBRACE block _RBRACE",
"rule : SWITCH list _LBRACE cases _RBRACE",
"rule : IF expr _LBRACE block _RBRACE",
"rule : MODULE list _LBRACE block _RBRACE",
"rule : WHILE expr _LBRACE block _RBRACE",
"rule : IF expr _LBRACE block _RBRACE ELSE rule",
"rule : local_opt RULE ARG arglist_opt rule",
"rule : ON arg rule",
"$$1 :",
"$$2 :",
"rule : ACTIONS eflags ARG bindlist _LBRACE $$1 STRING $$2 _RBRACE",
"assign : _EQUALS",
"assign : _PLUS_EQUALS",
"assign : _QUESTION_EQUALS",
"assign : DEFAULT _EQUALS",
"expr : arg",
"expr : arg _EQUALS arg",
"expr : arg _BANG_EQUALS arg",
"expr : arg _LANGLE arg",
"expr : arg _LANGLE_EQUALS arg",
"expr : arg _RANGLE arg",
"expr : arg _RANGLE_EQUALS arg",
"expr : expr _AMPER expr",
"expr : expr _AMPERAMPER expr",
"expr : expr _BAR expr",
"expr : expr _BARBAR expr",
"expr : arg IN list",
"expr : _BANG expr",
"expr : _LPAREN expr _RPAREN",
"cases :",
"cases : case cases",
"case : CASE ARG _COLON block",
"lol : list",
"lol : list _COLON lol",
"list : listp",
"listp :",
"listp : listp arg",
"arg : ARG",
"$$3 :",
"arg : _LBRACKET $$3 func _RBRACKET",
"func : ARG lol",
"func : ON arg ARG lol",
"func : ON arg RETURN list",
"eflags :",
"eflags : eflags eflag",
"eflag : UPDATED",
"eflag : TOGETHER",
"eflag : IGNORE",
"eflag : QUIETLY",
"eflag : PIECEMEAL",
"eflag : EXISTING",
"bindlist :",
"bindlist : BIND list",
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
#line 141 "jamgram.y"
{ parse_save( yyvsp[0].parse ); }
break;
case 3:
#line 152 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 4:
#line 154 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 5:
#line 158 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 6:
#line 160 "jamgram.y"
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 7:
#line 162 "jamgram.y"
{ yyval.parse = plocal( yyvsp[-3].parse, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 8:
#line 166 "jamgram.y"
{ yyval.parse = pnull(); }
break;
case 9:
#line 170 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_SET; }
break;
case 10:
#line 172 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_APPEND; }
break;
case 11:
#line 176 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 12:
#line 178 "jamgram.y"
{ yyval.parse = P0; }
break;
case 13:
#line 182 "jamgram.y"
{ yyval.number = 1; }
break;
case 14:
#line 184 "jamgram.y"
{ yyval.number = 0; }
break;
case 15:
#line 188 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 16:
#line 190 "jamgram.y"
{ yyval.parse = pincl( yyvsp[-1].parse ); }
break;
case 17:
#line 192 "jamgram.y"
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); }
break;
case 18:
#line 194 "jamgram.y"
{ yyval.parse = pset( yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
break;
case 19:
#line 196 "jamgram.y"
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
break;
case 20:
#line 198 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 21:
#line 200 "jamgram.y"
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-6].number ); }
break;
case 22:
#line 202 "jamgram.y"
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 23:
#line 204 "jamgram.y"
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); }
break;
case 24:
#line 206 "jamgram.y"
{ yyval.parse = pmodule( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 25:
#line 208 "jamgram.y"
{ yyval.parse = pwhile( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 26:
#line 210 "jamgram.y"
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); }
break;
case 27:
#line 212 "jamgram.y"
{ yyval.parse = psetc( yyvsp[-2].string, yyvsp[0].parse, yyvsp[-1].parse, yyvsp[-4].number ); }
break;
case 28:
#line 214 "jamgram.y"
{ yyval.parse = pon( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 29:
#line 216 "jamgram.y"
{ yymode( SCAN_STRING ); }
break;
case 30:
#line 218 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 31:
#line 220 "jamgram.y"
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); }
break;
case 32:
#line 228 "jamgram.y"
{ yyval.number = ASSIGN_SET; }
break;
case 33:
#line 230 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; }
break;
case 34:
#line 232 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 35:
#line 234 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 36:
#line 241 "jamgram.y"
{ yyval.parse = peval( EXPR_EXISTS, yyvsp[0].parse, pnull() ); }
break;
case 37:
#line 243 "jamgram.y"
{ yyval.parse = peval( EXPR_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 38:
#line 245 "jamgram.y"
{ yyval.parse = peval( EXPR_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 39:
#line 247 "jamgram.y"
{ yyval.parse = peval( EXPR_LESS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 40:
#line 249 "jamgram.y"
{ yyval.parse = peval( EXPR_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 41:
#line 251 "jamgram.y"
{ yyval.parse = peval( EXPR_MORE, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 42:
#line 253 "jamgram.y"
{ yyval.parse = peval( EXPR_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 43:
#line 255 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 44:
#line 257 "jamgram.y"
{ yyval.parse = pshortcircuiteval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 45:
#line 259 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 46:
#line 261 "jamgram.y"
{ yyval.parse = pshortcircuiteval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 47:
#line 263 "jamgram.y"
{ yyval.parse = peval( EXPR_IN, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 48:
#line 265 "jamgram.y"
{ yyval.parse = peval( EXPR_NOT, yyvsp[0].parse, pnull() ); }
break;
case 49:
#line 267 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 50:
#line 278 "jamgram.y"
{ yyval.parse = P0; }
break;
case 51:
#line 280 "jamgram.y"
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 52:
#line 284 "jamgram.y"
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); }
break;
case 53:
#line 293 "jamgram.y"
{ yyval.parse = pnode( P0, yyvsp[0].parse ); }
break;
case 54:
#line 295 "jamgram.y"
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); }
break;
case 55:
#line 305 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); }
break;
case 56:
#line 309 "jamgram.y"
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); }
break;
case 57:
#line 311 "jamgram.y"
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 58:
#line 315 "jamgram.y"
{ yyval.parse = plist( yyvsp[0].string ); }
break;
case 59:
#line 316 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 60:
#line 317 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 61:
#line 326 "jamgram.y"
{ yyval.parse = prule( yyvsp[-1].string, yyvsp[0].parse ); }
break;
case 62:
#line 328 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, prule( yyvsp[-1].string, yyvsp[0].parse ) ); }
break;
case 63:
#line 330 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 64:
#line 340 "jamgram.y"
{ yyval.number = 0; }
break;
case 65:
#line 342 "jamgram.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
break;
case 66:
#line 346 "jamgram.y"
{ yyval.number = EXEC_UPDATED; }
break;
case 67:
#line 348 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; }
break;
case 68:
#line 350 "jamgram.y"
{ yyval.number = EXEC_IGNORE; }
break;
case 69:
#line 352 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; }
break;
case 70:
#line 354 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; }
break;
case 71:
#line 356 "jamgram.y"
{ yyval.number = EXEC_EXISTING; }
break;
case 72:
#line 365 "jamgram.y"
{ yyval.parse = pnull(); }
break;
case 73:
#line 367 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
#line 861 "y.tab.c"
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
