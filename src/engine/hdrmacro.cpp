/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

/*
 * hdrmacro.c - handle header files that define macros used in #include
 *              statements.
 *
 *  we look for lines like "#define MACRO  <....>" or '#define MACRO  "    "' in
 *  the target file. When found, we then phony up a rule invocation like:
 *
 *  $(HDRRULE) <target> : <resolved included files> ;
 *
 * External routines:
 *    headers1() - scan a target for "#include MACRO" lines and try to resolve
 *                 them when needed
 *
 * Internal routines:
 *    headers1() - using regexp, scan a file and build include LIST
 */

#include "jam.h"
#include "hdrmacro.h"

#include "compile.h"
#include "hash.h"
#include "lists.h"
#include "object.h"
#include "parse.h"
#include "rules.h"
#include "jam_strings.h"
#include "regexp.h"
#include "variable.h"
#include "output.h"

#include <errno.h>
#include <string.h>

/* this type is used to store a dictionary of file header macros */
typedef struct header_macro
{
  OBJECT * symbol;
  OBJECT * filename;  /* we could maybe use a LIST here ?? */
} HEADER_MACRO;

static struct hash * header_macros_hash = 0;


/*
 * headers() - scan a target for include files and call HDRRULE
 */

#define MAXINC 10

void macro_headers( TARGET * t )
{
    /* This regexp is used to detect lines of the form
     * "#define  MACRO  <....>" or "#define  MACRO  "....."
     * in the header macro files.
     */
    b2::regex::program re(
        "^[     ]*#[    ]*define[   ]*([A-Za-z][A-Za-z0-9_]*)[  ]*"
        "[<\"]([^\">]*)[\">].*$" );

    FILE * f;
    char buf[ 1024 ];

    if ( is_debug_header() )
        out_printf( "macro header scan for %s\n", object_str( t->name ) );

    if ( !( f = fopen( object_str( t->boundname ), "r" ) ) )
    {
        err_printf( "[errno %d] failed to scan include file '%s': %s",
            errno, object_str( t->boundname ), strerror(errno) );
        return;
    }

    while ( fgets( buf, sizeof( buf ), f ) )
    {
        HEADER_MACRO var;
        HEADER_MACRO * v = &var;
        auto re_i = re.search(buf);

        if ( re_i && re_i[1].begin() )
        {
            OBJECT * symbol;
            int found;
            /* we detected a line that looks like "#define  MACRO  filename */
            std::string macro(re_i[1].begin(), re_i[1].end());
            std::string filename(re_i[2].begin(), re_i[2].end());

            if ( is_debug_header() )
                out_printf( "macro '%s' used to define filename '%s' in '%s'\n",
                    macro.c_str(), filename.c_str(), object_str( t->boundname )
                    );

            /* add macro definition to hash table */
            if ( !header_macros_hash )
                header_macros_hash = hashinit( sizeof( HEADER_MACRO ),
                    "hdrmacros" );

            symbol = object_new( macro.c_str() );
            v = (HEADER_MACRO *)hash_insert( header_macros_hash, symbol, &found
                );
            if ( !found )
            {
                v->symbol = symbol;
                v->filename = object_new( filename.c_str() );  /* never freed */
            }
            else
                object_free( symbol );
            /* XXXX: FOR NOW, WE IGNORE MULTIPLE MACRO DEFINITIONS !! */
            /*       WE MIGHT AS WELL USE A LIST TO STORE THEM..      */
        }
    }

    fclose( f );
}


OBJECT * macro_header_get( OBJECT * macro_name )
{
    HEADER_MACRO * v;
    if ( header_macros_hash && ( v = (HEADER_MACRO *)hash_find(
        header_macros_hash, macro_name ) ) )
    {
        if ( is_debug_header() )
            out_printf( "### macro '%s' evaluated to '%s'\n", object_str( macro_name
                ), object_str( v->filename ) );
        return v->filename;
    }
    return 0;
}
