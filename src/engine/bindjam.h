/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_BINDJAM_H
#define B2_BINDJAM_H

#include "bind.h"
#include "object.h"

namespace b2
{

/*
Binds all the declared C++ interfaces to Jam equivalents.
*/
void bind_jam();

/*
Jam language C++ interface binder.
*/
struct jam_binder : bind::binder_<jam_binder>
{
    /*
    Bind the named module to Jam. All this does for Jam is define the module
    scope in Jam.
    */
    void bind_module(
        const char *module_name);

    /*
    Register the class "meta-class". Which in the case of jam is a module
    with the rules and variables that gets imported and copied into the
    specific instance module as needed.
    */
    template <typename Class, typename... Bases>
    void bind_class(
        const char *module_name, const char *class_name,
        ::b2::bind::type_<Class>,
        ::b2::bind::type_<Bases>...);

    /*
    Bind the init ctor function `i` as a constructor for the class.
    */
    template <typename Class, typename Init>
    void bind_init(
        const char *module_name, const char *class_name,
        Class *c, Init i);

    /*
    Bind the given method of a class. THe function can be any invocable whose
    interface will be reflected to Jam.
    */
    template <typename Function>
    void bind_method(
        const char *module_name, const char *class_name,
        const char *method_name, Function f);

    /*
    Bind a module scoped plain function, i.e. Jam rule.
    */
    template <typename Function>
    void bind_function(
        const char *module_name,
        const char *function_name, Function f);
};

/*
Marshaling of arguments and results end up calling one of `from_jam` or
`to_jam` functions. These are specialized on some core types as needed:
`std::string`, `unsigned int`, `int`, `bool`
*/

template <typename Value>
Value from_jam(OBJECT *jam_value);

template <typename Value>
OBJECT *to_jam(Value value);

} // namespace b2

#endif
