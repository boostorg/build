/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PARSE_DWA20011020_H
# define PARSE_DWA20011020_H
# include "frames.h"
# include "modules.h"
# include "lists.h"

/*
 * parse.h - make and destroy parse trees as driven by the parser
 */

/*
 * parse tree node
 */

struct _PARSE {
    LIST    *(*func)( PARSE *p, FRAME *frame );
    PARSE   *left;
    PARSE   *right;
    PARSE   *third;
    char    *string;
    char    *string1;
    int      num;
    int      refs;
/*    module*  module; */
    char*    rulename;
    char*    file;
    int      line;
} ;

void    parse_file( char *f, FRAME* frame );
void    parse_save( PARSE *p );

PARSE * parse_make( 
    LIST    *(*func)( PARSE *p, FRAME* frame ),
    PARSE   *left,
    PARSE   *right,
    PARSE   *third,
    char    *string,
    char    *string1,
    int      num );

void    parse_refer( PARSE *p );
void    parse_free( PARSE *p );
LIST*   parse_evaluate( PARSE *p, FRAME* frame );

#endif

