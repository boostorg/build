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
# define psetmodule( l,r,a ) 	parse_make( compile_set_module,l,r,P0,S0,S0,a )
# define pset1( l,r,t,a )	parse_make( compile_settings,l,r,t,S0,S0,a )
# define psetc( s,p,a,l )     	parse_make( compile_setcomp,p,a,P0,s,S0,l )
# define psete( s,l,s1,f ) 	parse_make( compile_setexec,l,P0,P0,s,s1,f )
# define pswitch( l,r )   	parse_make( compile_switch,l,r,P0,S0,S0,0 )

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

#line 54 "y.tab.c"
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
    4,    4,    4,    4,    4,    4,    4,    4,    4,   16,
   17,    4,   11,   11,   11,   11,   13,   13,   13,   13,
   13,   13,   13,   13,   13,   13,   13,   13,   13,   13,
   12,   12,   18,    8,    8,    5,   19,   19,   10,   21,
   10,   20,   20,   20,   14,   14,   22,   22,   22,   22,
   22,   22,   15,   15,
};
const short yylen[] = {                                         2,
    0,    1,    1,    1,    1,    2,    5,    0,    2,    1,
    3,    0,    1,    0,    3,    3,    3,    4,    5,    6,
    3,    8,    5,    5,    5,    5,    7,    5,    3,    0,
    0,    9,    1,    1,    1,    2,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    2,    3,
    0,    2,    4,    1,    3,    1,    0,    2,    1,    0,
    4,    2,    4,    4,    0,    2,    1,    1,    1,    1,
    1,    1,    0,    2,
};
const short yydefred[] = {                                      0,
   60,   65,    0,    0,   57,    0,    0,    0,   57,   57,
    0,    0,    0,    0,    2,    0,    0,    0,    0,    0,
   13,    0,    0,    0,   59,    0,    0,    0,    0,    0,
   57,    0,    0,    0,    0,    0,    4,    0,    3,    0,
    0,    6,    0,   34,   33,   35,    0,   57,   57,    0,
   57,    0,   72,   69,   71,   70,   68,   67,    0,   66,
    0,   49,    0,    0,    0,    0,    0,    0,    0,   57,
    0,    0,    0,    0,    0,   16,   58,   57,   10,    0,
    0,    0,   29,   21,    0,    0,   15,   57,   17,    0,
   36,    0,    0,    0,   62,   61,   57,    0,   57,   50,
   39,   40,   41,   38,   42,   43,   48,   44,   45,    0,
    0,    0,    9,    0,    0,    0,    0,    0,    0,    0,
   55,   57,    0,   57,   18,   57,   57,   74,   30,    0,
    0,    7,   19,   25,    0,   23,   52,   26,    0,   28,
    0,   64,   63,    0,    0,    0,    0,   11,   20,   31,
    0,   27,   53,    0,   22,   32,
};
const short yydgoto[] = {                                      14,
   37,   38,   39,   16,   40,   80,  123,   41,   17,   18,
   49,  118,   27,   20,   98,  144,  154,  119,   29,   52,
   19,   60,
};
const short yysindex[] = {                                    -88,
    0,    0, -269, -248,    0,    0, -257, -229,    0,    0,
 -248,  -88,    0,    0,    0,  -88, -254, -160, -267,  -44,
    0, -258, -248, -248,    0,  -36, -212, -219, -229, -200,
    0, -222,  -71, -188, -215, -209,    0, -203,    0, -166,
 -155,    0, -201,    0,    0,    0, -168,    0,    0, -229,
    0, -152,    0,    0,    0,    0,    0,    0, -170,    0,
 -159,    0, -153, -229, -229, -229, -229, -229, -229,    0,
 -248, -248,  -88, -248, -248,    0,    0,    0,    0, -140,
 -200,  -88,    0,    0, -146,  -88,    0,    0,    0, -135,
    0,  -20, -124, -245,    0,    0,    0, -147,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -141,
 -165, -165,    0,  -88, -105, -137, -148, -136, -146, -133,
    0,    0,  -71,    0,    0,    0,    0,    0,    0, -132,
 -109,    0,    0,    0,  -94,    0,    0,    0,  -87,    0,
  -84,    0,    0, -112,  -88,  -71,  -88,    0,    0,    0,
 -108,    0,    0, -104,    0,    0,
};
const short yyrindex[] = {                                      7,
    0,    0, -110,    0,    0, -115, -238,    0,    0,    0,
    0, -198, -149,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -237,    0,    0, -233,  -60,
    0,    0,  -97,    0,    0,    0,    0,    0,    0,  -45,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -96,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -198,    0,    0,    0,    0,    0,    0,    0,
  -60, -198,    0,    0,  -93, -198,    0,    0,    0, -113,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -150, -142,    0,    4,    0,    0,    0,    0,  -93,    0,
    0,    0,  -97,    0,    0,    0,    0,    0,    0,    0,
    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -198,  -97, -111,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
   41,  -65,  -25,  -33,    5,  130,    0,  -49,  215,   68,
  131,  107,   -5,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 303
const short yytable[] = {                                      83,
   24,   95,    5,    8,   79,   36,    1,  110,   23,   28,
   30,   32,   24,   34,   35,   21,  116,   62,   63,   50,
  120,   37,   37,    1,   37,   56,   56,   31,   56,   56,
   56,   56,   51,   57,   56,   81,   43,   56,  121,   56,
   15,   61,    1,   56,  126,   76,   71,   72,  132,   71,
   72,   25,   92,   93,  127,   79,   42,   57,   37,   37,
   37,   57,   56,   56,   56,  108,  109,   78,  111,  112,
   25,   26,  139,   82,  107,   33,   84,  143,   26,  151,
   85,  153,  113,   73,   74,   75,   86,   74,   75,  140,
   26,   26,   14,   71,   72,   87,   77,   88,   90,   91,
    8,  128,   44,  130,   97,   71,   72,   45,  100,   89,
   46,   46,  152,   59,   57,   57,   47,   94,   59,   47,
   96,   59,   57,   99,  114,  122,   48,   59,  141,  117,
  142,  101,  102,  103,  104,  105,  106,   59,   26,   26,
  125,   26,   26,   74,   75,   46,   46,   46,  129,   57,
   57,  135,   57,   47,   47,   47,   57,  131,   12,  133,
   12,  134,  136,  145,    8,  138,   12,   12,  146,  147,
   12,   12,   12,   12,  148,   13,   12,   12,   12,   14,
  149,   12,   12,    1,   57,    2,   12,    8,  150,   14,
  155,    3,    4,   14,  156,    5,    6,    7,    8,   73,
    1,    9,    2,   10,    8,   51,   11,   12,    3,    4,
  115,   13,    5,   21,    7,    8,   54,   22,    9,   54,
   10,   64,  124,   11,   12,  137,    0,   54,   13,   65,
   66,   67,   68,   69,   53,    0,    0,   54,    0,    0,
    0,    0,   44,   55,   56,    0,   70,   45,   57,   58,
   46,    0,    0,    0,    0,   59,   47,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   24,    0,   24,    0,   24,    0,    5,    8,
   24,   24,    0,    0,   24,   24,   24,   24,    0,    0,
   24,   24,   24,   14,   14,   24,   24,   14,    0,   24,
   24,    5,    8,
};
const short yycheck[] = {                                      33,
    0,   51,    0,    0,   30,   11,    0,   73,  257,    5,
    6,    7,  261,    9,   10,  285,   82,   23,   24,  287,
   86,  259,  260,  272,  262,  259,  260,  285,  262,  263,
  264,  265,  300,  272,  268,   31,  291,  271,   88,  273,
    0,  300,  272,  277,  290,  265,  259,  260,  114,  259,
  260,  300,   48,   49,  300,   81,   16,  296,  296,  297,
  298,  300,  296,  297,  298,   71,   72,  268,   74,   75,
  300,    4,  122,  296,   70,    8,  265,  127,   11,  145,
  296,  147,   78,  296,  297,  298,  296,  297,  298,  123,
   23,   24,  291,  259,  260,  299,   29,  264,  300,  268,
  299,   97,  263,   99,  275,  259,  260,  268,  262,  265,
  271,  262,  146,  263,  264,  265,  277,   50,  268,  262,
  273,  271,  272,  283,  265,  261,  287,  277,  124,  276,
  126,   64,   65,   66,   67,   68,   69,  287,   71,   72,
  265,   74,   75,  297,  298,  296,  297,  298,  296,  265,
  300,  300,  268,  296,  297,  298,  272,  299,  272,  265,
  274,  299,  299,  296,  276,  299,  280,  281,  278,  264,
  284,  285,  286,  287,  262,  291,  290,  291,  292,  291,
  265,  295,  296,  272,  300,  274,  300,  299,  301,  300,
  299,  280,  281,  291,  299,  284,  285,  286,  287,  296,
  272,  290,  274,  292,  265,  299,  295,  296,  280,  281,
   81,  300,  284,  285,  286,  287,  262,    3,  290,  265,
  292,  258,   92,  295,  296,  119,   -1,  273,  300,  266,
  267,  268,  269,  270,  279,   -1,   -1,  282,   -1,   -1,
   -1,   -1,  263,  288,  289,   -1,  283,  268,  293,  294,
  271,   -1,   -1,   -1,   -1,  300,  277,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,  274,   -1,  276,   -1,  276,  276,
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
"rule : MODULE LOCAL list assign_list_opt _SEMIC",
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
{ yyval.parse = psetmodule( yyvsp[-2].parse, yyvsp[-1].parse, yyvsp[-1].number ); }
break;
case 20:
#line 199 "jamgram.y"
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
break;
case 21:
#line 201 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 22:
#line 203 "jamgram.y"
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-6].number ); }
break;
case 23:
#line 205 "jamgram.y"
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 24:
#line 207 "jamgram.y"
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); }
break;
case 25:
#line 209 "jamgram.y"
{ yyval.parse = pmodule( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 26:
#line 211 "jamgram.y"
{ yyval.parse = pwhile( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 27:
#line 213 "jamgram.y"
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); }
break;
case 28:
#line 215 "jamgram.y"
{ yyval.parse = psetc( yyvsp[-2].string, yyvsp[0].parse, yyvsp[-1].parse, yyvsp[-4].number ); }
break;
case 29:
#line 217 "jamgram.y"
{ yyval.parse = pon( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 30:
#line 219 "jamgram.y"
{ yymode( SCAN_STRING ); }
break;
case 31:
#line 221 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 32:
#line 223 "jamgram.y"
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); }
break;
case 33:
#line 231 "jamgram.y"
{ yyval.number = ASSIGN_SET; }
break;
case 34:
#line 233 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; }
break;
case 35:
#line 235 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 36:
#line 237 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 37:
#line 244 "jamgram.y"
{ yyval.parse = peval( EXPR_EXISTS, yyvsp[0].parse, pnull() ); }
break;
case 38:
#line 246 "jamgram.y"
{ yyval.parse = peval( EXPR_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 39:
#line 248 "jamgram.y"
{ yyval.parse = peval( EXPR_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 40:
#line 250 "jamgram.y"
{ yyval.parse = peval( EXPR_LESS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 41:
#line 252 "jamgram.y"
{ yyval.parse = peval( EXPR_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 42:
#line 254 "jamgram.y"
{ yyval.parse = peval( EXPR_MORE, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 43:
#line 256 "jamgram.y"
{ yyval.parse = peval( EXPR_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 44:
#line 258 "jamgram.y"
{ yyval.parse = peval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 45:
#line 260 "jamgram.y"
{ yyval.parse = pshortcircuiteval( EXPR_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 46:
#line 262 "jamgram.y"
{ yyval.parse = peval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 47:
#line 264 "jamgram.y"
{ yyval.parse = pshortcircuiteval( EXPR_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 48:
#line 266 "jamgram.y"
{ yyval.parse = peval( EXPR_IN, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 49:
#line 268 "jamgram.y"
{ yyval.parse = peval( EXPR_NOT, yyvsp[0].parse, pnull() ); }
break;
case 50:
#line 270 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 51:
#line 281 "jamgram.y"
{ yyval.parse = P0; }
break;
case 52:
#line 283 "jamgram.y"
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 53:
#line 287 "jamgram.y"
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); }
break;
case 54:
#line 296 "jamgram.y"
{ yyval.parse = pnode( P0, yyvsp[0].parse ); }
break;
case 55:
#line 298 "jamgram.y"
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); }
break;
case 56:
#line 308 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); }
break;
case 57:
#line 312 "jamgram.y"
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); }
break;
case 58:
#line 314 "jamgram.y"
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 59:
#line 318 "jamgram.y"
{ yyval.parse = plist( yyvsp[0].string ); }
break;
case 60:
#line 319 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 61:
#line 320 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 62:
#line 329 "jamgram.y"
{ yyval.parse = prule( yyvsp[-1].string, yyvsp[0].parse ); }
break;
case 63:
#line 331 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, prule( yyvsp[-1].string, yyvsp[0].parse ) ); }
break;
case 64:
#line 333 "jamgram.y"
{ yyval.parse = pon( yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 65:
#line 343 "jamgram.y"
{ yyval.number = 0; }
break;
case 66:
#line 345 "jamgram.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
break;
case 67:
#line 349 "jamgram.y"
{ yyval.number = EXEC_UPDATED; }
break;
case 68:
#line 351 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; }
break;
case 69:
#line 353 "jamgram.y"
{ yyval.number = EXEC_IGNORE; }
break;
case 70:
#line 355 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; }
break;
case 71:
#line 357 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; }
break;
case 72:
#line 359 "jamgram.y"
{ yyval.number = EXEC_EXISTING; }
break;
case 73:
#line 368 "jamgram.y"
{ yyval.parse = pnull(); }
break;
case 74:
#line 370 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
#line 867 "y.tab.c"
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
