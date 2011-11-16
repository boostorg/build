/*
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */
#include "jam.h"

#include "modules.h"
#include "string.h"
#include "hash.h"
#include "newstr.h"
#include "lists.h"
#include "parse.h"
#include "rules.h"
#include "variable.h"
#include "strings.h"
#include "native.h"
#include <assert.h>

static struct hash * module_hash = 0;


static char * new_module_str( module_t * m, char * suffix )
{
    char * result;
    string s;
    string_copy( &s, m->name );
    string_append( &s, suffix );
    result = newstr( s.value );
    string_free( &s );
    return result;
}


module_t * bindmodule( char * name )
{
    PROFILE_ENTER( BINDMODULE );

    string s;
    module_t m_;
    module_t * m = &m_;

    if ( !module_hash )
        module_hash = hashinit( sizeof( module_t ), "modules" );

    string_new( &s );
    if ( name )
    {
        string_append( &s, name );
        string_push_back( &s, '.' );
    }

    m->name = s.value;

    if ( hashenter( module_hash, (HASHDATA * *)&m ) )
    {
        m->name = newstr( m->name );
        m->variables = 0;
        m->rules = 0;
        m->imported_modules = 0;
        m->class_module = 0;
        m->native_rules = 0;
        m->user_module = 0;
    }
    string_free( &s );

    PROFILE_EXIT( BINDMODULE );

    return m;
}

/*
 * demand_rules() - Get the module's "rules" hash on demand.
 */
struct hash * demand_rules( module_t * m )
{
    if ( !m->rules )
        m->rules = hashinit( sizeof( RULE ), new_module_str( m, "rules" ) );
    return m->rules;
}


/*
 * delete_module() - wipe out the module's rules and variables.
 */

static void delete_rule_( void * xrule, void * data )
{
    rule_free( (RULE *)xrule );
}


static void delete_native_rule( void * xrule, void * data )
{
    native_rule_t * rule = (native_rule_t *)xrule;
    if ( rule->arguments )
        args_free( rule->arguments );
    freestr( rule->name );
    if ( rule->procedure )
        parse_free( rule->procedure );
}


static void delete_imported_modules( void * xmodule_name, void * data )
{
    freestr( *(char * *)xmodule_name );
}


void delete_module( module_t * m )
{
    /* Clear out all the rules. */
    if ( m->rules )
    {
        char * name;
        hashenumerate( m->rules, delete_rule_, (void *)0 );
        name = hashname( m->rules );
        hashdone( m->rules );
        freestr( name );
        m->rules = 0;
    }

    if ( m->native_rules )
    {
        char * name;
        hashenumerate( m->native_rules, delete_native_rule, (void *)0 );
        hashdone( m->native_rules );
        m->native_rules = 0;
    }

    if ( m->variables )
    {
        var_hash_swap( &m->variables );
        var_done();
        var_hash_swap( &m->variables );
        m->variables = 0;
    }

    if ( m->imported_modules )
    {
        hashenumerate( m->imported_modules, delete_imported_modules, (void *)0 );
        hashdone( m->imported_modules );
        m->imported_modules = 0;
    }
}


static void delete_module_( void * xmodule, void * data )
{
    module_t *m = (module_t *)xmodule;

    delete_module( m );

    if ( m->name )
    {
        freestr( m->name );
    }
}

void modules_done()
{
    hashenumerate( module_hash, delete_module_, (void *)0 ); 
    hashdone( module_hash );
    module_hash = 0;
}

module_t * root_module()
{
    static module_t * root = 0;
    if ( !root )
        root = bindmodule( 0 );
    return root;
}

void enter_module( module_t * m )
{
    var_hash_swap( &m->variables );
}


void exit_module( module_t * m )
{
    var_hash_swap( &m->variables );
}


void import_module( LIST * module_names, module_t * target_module )
{
    PROFILE_ENTER( IMPORT_MODULE );

    struct hash * h;

    if ( !target_module->imported_modules )
        target_module->imported_modules = hashinit( sizeof( char * ), "imported" );
    h = target_module->imported_modules;

    for ( ; module_names; module_names = module_names->next )
    {
        char * s = module_names->string;
        char * * ss = &s;
        if( hashenter( h, (HASHDATA * *)&ss ) )
        {
            *ss = copystr( s );
        }
    }

    PROFILE_EXIT( IMPORT_MODULE );
}


static void add_module_name( void * r_, void * result_ )
{
    char * * r = (char * *)r_;
    LIST * * result = (LIST * *)result_;

    *result = list_new( *result, copystr( *r ) );
}


LIST * imported_modules( module_t * module )
{
    LIST * result = L0;
    if ( module->imported_modules )
        hashenumerate( module->imported_modules, add_module_name, &result );
    return result;
}
