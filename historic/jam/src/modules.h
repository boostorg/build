/*  (C) Copyright David Abrahams 2001. Permission to copy, use, modify, sell and
 *  distribute this software is granted provided this copyright notice appears
 *  in all copies. This software is provided "as is" without express or implied
 *  warranty, and with no claim as to its suitability for any purpose.
 */
#ifndef MODULES_DWA10182001_H
# define MODULES_DWA10182001_H

struct module_t
{
    char* name;
    struct hash* rules;
    struct hash* variables;
};

typedef struct module_t module_t ; /* MSVC debugger gets confused unless this is provided */

module_t* bindmodule( char* name );
module_t* root_module();
void bind_module_var( module_t*, char* name );
void enter_module( module_t* );
void exit_module( module_t* );
void delete_module( module_t* );
struct hash* demand_rules( module_t* );

#endif

