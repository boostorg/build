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
#line 94 "jamgram.y"
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
# define psetmodule( l,r,a ) 	parse_make( compile_set_module,l,r,P0,S0,S0,a )
# define pset1( l,r,t,a )	parse_make( compile_settings,l,r,t,S0,S0,a )
# define psetc( s,p,a,l )     	parse_make( compile_setcomp,p,a,P0,s,S0,l )
# define psete( s,l,s1,f ) 	parse_make( compile_setexec,l,P0,P0,s,s1,f )
# define pswitch( l,r )   	parse_make( compile_switch,l,r,P0,S0,S0,0 )

# define pnode( l,r )    	parse_make( F0,l,r,P0,S0,S0,0 )
# define pcnode( c,l,r )	parse_make( F0,l,r,P0,S0,S0,c )
# define psnode( s,l )     	parse_make( F0,l,P0,P0,s,S0,0 )

#line 52 "y.tab.c"
#define YYERRCODE 256
#define _BANG 257
#define _BANG_EQUALS 258
#define _AMPERAMPER 259
#define _LPAREN 260
#define _RPAREN 261
#define _PLUS_EQUALS 262
#define _COLON 263
#define _SEMIC 264
#define _LANGLE 265
#define _LANGLE_EQUALS 266
#define _EQUALS 267
#define _RANGLE 268
#define _RANGLE_EQUALS 269
#define _QUESTION_EQUALS 270
#define _LBRACKET 271
#define _RBRACKET 272
#define ACTIONS 273
#define BIND 274
#define CASE 275
#define DEFAULT 276
#define ELSE 277
#define EXISTING 278
#define FOR 279
#define IF 280
#define IGNORE 281
#define IN 282
#define INCLUDE 283
#define LOCAL 284
#define MODULE 285
#define ON 286
#define PIECEMEAL 287
#define QUIETLY 288
#define RETURN 289
#define RULE 290
#define SWITCH 291
#define TOGETHER 292
#define UPDATED 293
#define WHILE 294
#define _LBRACE 295
#define _BARBAR 296
#define _RBRACE 297
#define ARG 298
#define STRING 299
const short yylhs[] = {                                        -1,
    0,    0,    2,    2,    1,    1,    1,    3,    6,    6,
    7,    7,    9,    9,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,   16,   17,
    4,   11,   11,   11,   11,   13,   13,   13,   13,   13,
   13,   13,   13,   13,   13,   13,   13,   12,   12,   18,
    8,    8,    5,   19,   19,   10,   10,   14,   14,   20,
   20,   20,   20,   20,   20,   15,   15,
};
const short yylen[] = {                                         2,
    0,    1,    1,    1,    1,    2,    5,    0,    2,    1,
    3,    0,    1,    0,    3,    3,    3,    4,    5,    6,
    3,    8,    5,    5,    5,    5,    7,    5,    0,    0,
    9,    1,    1,    1,    2,    1,    3,    3,    3,    3,
    3,    3,    3,    2,    3,    3,    3,    0,    2,    4,
    1,    3,    1,    0,    2,    1,    4,    0,    2,    1,
    1,    1,    1,    1,    1,    0,    2,
};
const short yydefred[] = {                                      0,
    0,   58,    0,    0,   54,    0,    0,   54,   54,    0,
    0,    0,    0,    2,    0,    0,    0,   54,    0,   13,
    0,    0,    0,   56,    0,    0,    0,    0,    0,   54,
    0,    0,    0,    0,    4,    0,    3,    0,    0,    6,
    0,   33,   32,   34,    0,   54,   54,    0,   65,   62,
   64,   63,   61,   60,    0,   59,    0,   44,    0,    0,
    0,    0,    0,    0,    0,   54,    0,    0,    0,   16,
   55,   54,   10,    0,    0,    0,   21,    0,    0,   15,
   54,   17,    0,   35,    0,    0,   57,   54,    0,   54,
   47,   38,   39,   40,   37,   41,   42,   43,   45,    0,
    0,    9,    0,    0,    0,    0,    0,    0,    0,   52,
   54,    0,   54,   18,   67,   29,    0,    0,    7,   19,
   25,    0,   23,   49,   26,    0,   28,    0,    0,    0,
    0,    0,   11,   20,   30,    0,   27,   50,    0,   22,
   31,
};
const short yydgoto[] = {                                      13,
   35,   36,   37,   15,   38,   74,  112,   39,   16,   17,
   47,  107,   26,   19,   89,  129,  139,  108,   28,   56,
};
const short yysindex[] = {                                   -120,
 -282,    0, -267, -234,    0,    0, -259,    0,    0, -234,
 -120,    0,    0,    0, -120, -261,  -83,    0, -131,    0,
 -236, -234, -234,    0,  -69, -250, -181, -244, -188,    0,
 -210, -175, -204, -209,    0, -205,    0, -157, -154,    0,
 -184,    0,    0,    0, -152,    0,    0, -156,    0,    0,
    0,    0,    0,    0, -153,    0, -158,    0, -185, -244,
 -244, -244, -244, -244, -244,    0, -234, -120, -234,    0,
    0,    0,    0, -135, -188, -120,    0, -144, -120,    0,
    0,    0, -128,    0, -150, -124,    0,    0, -149,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -155,
 -118,    0, -120, -110, -139, -143, -125, -144, -114,    0,
    0, -103,    0,    0,    0,    0, -129, -104,    0,    0,
    0,  -78,    0,    0,    0,  -71,    0,  -70,  -98, -120,
 -103, -120,    0,    0,    0,  -95,    0,    0,  -93,    0,
    0,
};
const short yyrindex[] = {                                      7,
    0,    0,  -92,    0,    0, -162, -201,    0,    0,    0,
 -275, -163,    0,    0,    3,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -229,    0,    0, -223,  -59,    0,
    0,    0,    0,    0,    0,    0,    0, -142,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -88,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -275,    0,    0,
    0,    0,    0,    0,  -59, -275,    0,  -89, -275,    0,
    0,    0, -146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -253,    0,    4,    0,    0,    0,    0,  -89,    0,    0,
    0,  -81,    0,    0,    0,    0,    0,    1,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -275,
  -81, -194,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
const short yygindex[] = {                                      0,
   75,  -48,  -27,  -43,    5,  135,    0,  -13,  208,   -4,
  127,  106,   11,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 301
const short yytable[] = {                                      25,
   24,   73,    5,    8,   48,   25,    1,   46,   67,   27,
   29,   31,   32,   33,   14,   18,   20,   25,   25,  100,
   34,    8,   22,   71,   30,   23,    1,  105,   41,   36,
  109,   36,   58,   59,   75,   53,    1,   53,   53,   53,
   53,   46,   46,   53,   68,   69,   53,   73,   53,   67,
   85,   86,   53,   24,  119,   92,   93,   94,   95,   96,
   97,   57,   25,   24,   25,   36,   36,  110,  127,   54,
   98,   53,   53,   67,   14,   91,  102,   99,   72,  101,
    8,  136,   70,  138,   76,   79,   69,  137,   77,   40,
   78,   80,  115,   54,  117,   14,   54,  126,   56,   54,
   54,   54,    8,   56,   54,   81,   56,   54,   54,   82,
   69,   42,   56,   83,   84,   87,   43,  128,   51,   44,
   88,   51,   56,   90,   12,   45,   12,   13,  103,   51,
  106,  111,   12,   12,   54,   54,   12,   12,   12,  114,
   67,  118,   12,   12,   12,  116,   49,   12,   12,   50,
    1,   12,    2,  120,  122,   51,   52,  121,    3,    4,
   53,   54,    5,    6,    7,  130,   55,    1,    8,    2,
    9,  123,  131,   10,   11,    3,    4,   12,   42,    5,
   20,    7,  125,   43,  132,    8,   44,    9,   60,  133,
   10,   11,   45,  134,   12,   61,   62,   63,   64,   65,
  135,  140,   46,  141,    8,   14,   66,   48,   14,  104,
   21,  113,   66,  124,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   24,    0,   24,    0,   24,    0,    5,    8,   24,
   24,    0,    0,   24,   24,   24,    0,    0,    0,   24,
   24,   24,   14,   14,   24,   24,   14,   24,   24,    5,
    8,
};
const short yycheck[] = {                                       4,
    0,   29,    0,    0,   18,   10,    0,  261,  259,    5,
    6,    7,    8,    9,  290,  298,  284,   22,   23,   68,
   10,  297,  257,   28,  284,  260,  271,   76,  290,  259,
   79,  261,   22,   23,   30,  259,  271,  261,  262,  263,
  264,  295,  296,  267,  295,  296,  270,   75,  272,  259,
   46,   47,  276,  298,  103,   60,   61,   62,   63,   64,
   65,  298,   67,  298,   69,  295,  296,   81,  112,  271,
   66,  295,  296,  259,    0,  261,   72,   67,  267,   69,
  275,  130,  264,  132,  295,  295,  296,  131,  264,   15,
  295,  297,   88,  295,   90,  290,  298,  111,  262,  263,
  264,  264,  297,  267,  267,  263,  270,  271,  271,  264,
  296,  262,  276,  298,  267,  272,  267,  113,  261,  270,
  274,  264,  286,  282,  271,  276,  273,  290,  264,  272,
  275,  260,  279,  280,  298,  298,  283,  284,  285,  264,
  259,  297,  289,  290,  291,  295,  278,  294,  295,  281,
  271,  298,  273,  264,  298,  287,  288,  297,  279,  280,
  292,  293,  283,  284,  285,  295,  298,  271,  289,  273,
  291,  297,  277,  294,  295,  279,  280,  298,  262,  283,
  284,  285,  297,  267,  263,  289,  270,  291,  258,  261,
  294,  295,  276,  264,  298,  265,  266,  267,  268,  269,
  299,  297,  286,  297,  264,  298,  295,  297,  290,   75,
    3,   85,  282,  108,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  271,   -1,  273,   -1,  275,   -1,  275,  275,  279,
  280,   -1,   -1,  283,  284,  285,   -1,   -1,   -1,  289,
  290,  291,  290,  290,  294,  295,  290,  297,  298,  297,
  297,
};
#define YYFINAL 13
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 299
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"_BANG","_BANG_EQUALS",
"_AMPERAMPER","_LPAREN","_RPAREN","_PLUS_EQUALS","_COLON","_SEMIC","_LANGLE",
"_LANGLE_EQUALS","_EQUALS","_RANGLE","_RANGLE_EQUALS","_QUESTION_EQUALS",
"_LBRACKET","_RBRACKET","ACTIONS","BIND","CASE","DEFAULT","ELSE","EXISTING",
"FOR","IF","IGNORE","IN","INCLUDE","LOCAL","MODULE","ON","PIECEMEAL","QUIETLY",
"RETURN","RULE","SWITCH","TOGETHER","UPDATED","WHILE","_LBRACE","_BARBAR",
"_RBRACE","ARG","STRING",
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
"rule : IF cond _LBRACE block _RBRACE",
"rule : MODULE list _LBRACE block _RBRACE",
"rule : WHILE cond _LBRACE block _RBRACE",
"rule : IF cond _LBRACE block _RBRACE ELSE rule",
"rule : local_opt RULE ARG arglist_opt rule",
"$$1 :",
"$$2 :",
"rule : ACTIONS eflags ARG bindlist _LBRACE $$1 STRING $$2 _RBRACE",
"assign : _EQUALS",
"assign : _PLUS_EQUALS",
"assign : _QUESTION_EQUALS",
"assign : DEFAULT _EQUALS",
"cond : arg",
"cond : arg _EQUALS arg",
"cond : arg _BANG_EQUALS arg",
"cond : arg _LANGLE arg",
"cond : arg _LANGLE_EQUALS arg",
"cond : arg _RANGLE arg",
"cond : arg _RANGLE_EQUALS arg",
"cond : arg IN list",
"cond : _BANG cond",
"cond : cond _AMPERAMPER cond",
"cond : cond _BARBAR cond",
"cond : _LPAREN cond _RPAREN",
"cases :",
"cases : case cases",
"case : CASE ARG _COLON block",
"lol : list",
"lol : list _COLON lol",
"list : listp",
"listp :",
"listp : listp arg",
"arg : ARG",
"arg : _LBRACKET ARG lol _RBRACKET",
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
#line 135 "jamgram.y"
{ parse_save( yyvsp[0].parse ); }
break;
case 3:
#line 146 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 4:
#line 148 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 5:
#line 152 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
case 6:
#line 154 "jamgram.y"
{ yyval.parse = prules( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 7:
#line 156 "jamgram.y"
{ yyval.parse = plocal( yyvsp[-3].parse, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 8:
#line 160 "jamgram.y"
{ yyval.parse = pnull(); }
break;
case 9:
#line 164 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_SET; }
break;
case 10:
#line 166 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yyval.number = ASSIGN_APPEND; }
break;
case 11:
#line 170 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 12:
#line 172 "jamgram.y"
{ yyval.parse = P0; }
break;
case 13:
#line 176 "jamgram.y"
{ yyval.number = 1; }
break;
case 14:
#line 178 "jamgram.y"
{ yyval.number = 0; }
break;
case 15:
#line 182 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 16:
#line 184 "jamgram.y"
{ yyval.parse = pincl( yyvsp[-1].parse ); }
break;
case 17:
#line 186 "jamgram.y"
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); }
break;
case 18:
#line 188 "jamgram.y"
{ yyval.parse = pset( yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
break;
case 19:
#line 190 "jamgram.y"
{ yyval.parse = psetmodule( yyvsp[-2].parse, yyvsp[-1].parse, yyvsp[-1].number ); }
break;
case 20:
#line 192 "jamgram.y"
{ yyval.parse = pset1( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-2].number ); }
break;
case 21:
#line 194 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 22:
#line 196 "jamgram.y"
{ yyval.parse = pfor( yyvsp[-5].string, yyvsp[-3].parse, yyvsp[-1].parse, yyvsp[-6].number ); }
break;
case 23:
#line 198 "jamgram.y"
{ yyval.parse = pswitch( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 24:
#line 200 "jamgram.y"
{ yyval.parse = pif( yyvsp[-3].parse, yyvsp[-1].parse, pnull() ); }
break;
case 25:
#line 202 "jamgram.y"
{ yyval.parse = pmodule( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 26:
#line 204 "jamgram.y"
{ yyval.parse = pwhile( yyvsp[-3].parse, yyvsp[-1].parse ); }
break;
case 27:
#line 206 "jamgram.y"
{ yyval.parse = pif( yyvsp[-5].parse, yyvsp[-3].parse, yyvsp[0].parse ); }
break;
case 28:
#line 208 "jamgram.y"
{ yyval.parse = psetc( yyvsp[-2].string, yyvsp[0].parse, yyvsp[-1].parse, yyvsp[-4].number ); }
break;
case 29:
#line 210 "jamgram.y"
{ yymode( SCAN_STRING ); }
break;
case 30:
#line 212 "jamgram.y"
{ yymode( SCAN_NORMAL ); }
break;
case 31:
#line 214 "jamgram.y"
{ yyval.parse = psete( yyvsp[-6].string,yyvsp[-5].parse,yyvsp[-2].string,yyvsp[-7].number ); }
break;
case 32:
#line 222 "jamgram.y"
{ yyval.number = ASSIGN_SET; }
break;
case 33:
#line 224 "jamgram.y"
{ yyval.number = ASSIGN_APPEND; }
break;
case 34:
#line 226 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 35:
#line 228 "jamgram.y"
{ yyval.number = ASSIGN_DEFAULT; }
break;
case 36:
#line 236 "jamgram.y"
{ yyval.parse = pcnode( COND_EXISTS, yyvsp[0].parse, pnull() ); }
break;
case 37:
#line 238 "jamgram.y"
{ yyval.parse = pcnode( COND_EQUALS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 38:
#line 240 "jamgram.y"
{ yyval.parse = pcnode( COND_NOTEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 39:
#line 242 "jamgram.y"
{ yyval.parse = pcnode( COND_LESS, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 40:
#line 244 "jamgram.y"
{ yyval.parse = pcnode( COND_LESSEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 41:
#line 246 "jamgram.y"
{ yyval.parse = pcnode( COND_MORE, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 42:
#line 248 "jamgram.y"
{ yyval.parse = pcnode( COND_MOREEQ, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 43:
#line 250 "jamgram.y"
{ yyval.parse = pcnode( COND_IN, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 44:
#line 252 "jamgram.y"
{ yyval.parse = pcnode( COND_NOT, yyvsp[0].parse, P0 ); }
break;
case 45:
#line 254 "jamgram.y"
{ yyval.parse = pcnode( COND_AND, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 46:
#line 256 "jamgram.y"
{ yyval.parse = pcnode( COND_OR, yyvsp[-2].parse, yyvsp[0].parse ); }
break;
case 47:
#line 258 "jamgram.y"
{ yyval.parse = yyvsp[-1].parse; }
break;
case 48:
#line 268 "jamgram.y"
{ yyval.parse = P0; }
break;
case 49:
#line 270 "jamgram.y"
{ yyval.parse = pnode( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 50:
#line 274 "jamgram.y"
{ yyval.parse = psnode( yyvsp[-2].string, yyvsp[0].parse ); }
break;
case 51:
#line 283 "jamgram.y"
{ yyval.parse = pnode( P0, yyvsp[0].parse ); }
break;
case 52:
#line 285 "jamgram.y"
{ yyval.parse = pnode( yyvsp[0].parse, yyvsp[-2].parse ); }
break;
case 53:
#line 295 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; yymode( SCAN_NORMAL ); }
break;
case 54:
#line 299 "jamgram.y"
{ yyval.parse = pnull(); yymode( SCAN_PUNCT ); }
break;
case 55:
#line 301 "jamgram.y"
{ yyval.parse = pappend( yyvsp[-1].parse, yyvsp[0].parse ); }
break;
case 56:
#line 305 "jamgram.y"
{ yyval.parse = plist( yyvsp[0].string ); }
break;
case 57:
#line 307 "jamgram.y"
{ yyval.parse = prule( yyvsp[-2].string, yyvsp[-1].parse ); }
break;
case 58:
#line 317 "jamgram.y"
{ yyval.number = 0; }
break;
case 59:
#line 319 "jamgram.y"
{ yyval.number = yyvsp[-1].number | yyvsp[0].number; }
break;
case 60:
#line 323 "jamgram.y"
{ yyval.number = EXEC_UPDATED; }
break;
case 61:
#line 325 "jamgram.y"
{ yyval.number = EXEC_TOGETHER; }
break;
case 62:
#line 327 "jamgram.y"
{ yyval.number = EXEC_IGNORE; }
break;
case 63:
#line 329 "jamgram.y"
{ yyval.number = EXEC_QUIETLY; }
break;
case 64:
#line 331 "jamgram.y"
{ yyval.number = EXEC_PIECEMEAL; }
break;
case 65:
#line 333 "jamgram.y"
{ yyval.number = EXEC_EXISTING; }
break;
case 66:
#line 342 "jamgram.y"
{ yyval.parse = pnull(); }
break;
case 67:
#line 344 "jamgram.y"
{ yyval.parse = yyvsp[0].parse; }
break;
#line 821 "y.tab.c"
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
