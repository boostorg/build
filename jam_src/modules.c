/*  (C) Copyright David Abrahams 2001. Permission to copy, use, modify, sell and
 *  distribute this software is granted provided this copyright notice appears
 *  in all copies. This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 */
#include "modules.h"
#include "jam.h"
#include "string.h"
#include "hash.h"
#include "newstr.h"
#include "lists.h"
#include "parse.h"
#include "rules.h"
#include "variable.h"
#include "strings.h"

static struct hash* module_hash = 0;

static char* new_module_str( module* m, char* suffix )
{
    char* result;
    string s;
    string_copy( &s, m->name );
    string_append( &s, suffix );
    result = newstr( s.value );
    string_free( &s );
    return result;
}

module* bindmodule( char* name )
{
    string s;
    module m_, *m = &m_;

    if( !module_hash )
        module_hash = hashinit( sizeof( module ), "modules" );

    string_new( &s );
    if (name)
    {
        string_append( &s, name );
        string_push_back( &s, '.' );
    }
        
    m->name = s.value;
    
    if ( hashenter( module_hash, (HASHDATA **)&m ) )
    {
        m->name = newstr( m->name );
        m->local_names = 0;
        m->locals = 0;
        m->rules = hashinit( sizeof( RULE ), new_module_str( m, "rules" ) );
    }
    string_free( &s );
    return m;
}

module* root_module()
{
    static module* root = 0;
    if ( !root )
        root = bindmodule(0);
    return root;
}

/*
 * bind_module_var --
 *
 * Add the symbol to the module's list of symbols if it is not already in the
 * module. m is assumed to be the current module and if the symbol is new, any
 * current value is replaced by an empty list until the module is exited.
 *
 */
void bind_module_var( module* m, char* symbol )
{
    char** name = &symbol;
    
    if ( !m->local_names )
        m->local_names = hashinit( sizeof( char* ), new_module_str( m, "variables" ) );
    
    if ( hashenter( m->local_names, (HASHDATA **)&name ) )
    {
        m->locals = addsettings( m->locals, 0, symbol, var_swap( symbol, 0 ) );
    }
}

void enter_module( module* m )
{
    pushsettings( m->locals );
}

void exit_module( module* m )
{
    popsettings( m->locals );
}
