/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_BIND_H
#define B2_BIND_H

#include "config.h"
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

/** tag::binder[]

= Binder

The B2 C++ native engine and system is reflected to various other languages
through bindings of classes and functions. This is accomplished through custom
C++ 11 reflection and non-intrusive declaration of a binding API.

end::binder[] */

namespace b2
{
namespace bind
{

/** tag::binder_type[]

== `b2::bind::type_`

Tag wrapper definition to specify a type to one of the binder specifications.
For example:

```
binder.def_class("system_info", type_<b2::system_info>());
```

end::binder_type[] */
template <typename T>
struct type_
{
    typedef T type;
};

/** tag::binder_init[]

== `b2::bind::init_`

Tag wrapper definition to specify the types of arguments for init methods,
i.e. constructors, to a binder definition.
For example:

```
binder.def_class("system_info", type_<b2::system_info>())
    .def(init_<>());
```

end::binder_init[] */
template <typename... Args>
struct init_
{
};

/** tag::binder_module[]

== `b2::bind::module_`

The base type for a module binding. Modules are a collection of class and other
binding declarations. A module is specified as a concrete class with at minimum
a `module_name` and a definition method
(`template <class Binder> void def(Binder & binder)`).
For example:

```
struct sysinfo_module
    : b2::bind::module_<sysinfo_module>
{
    const char *module_name = "sysinfo";

    template <class Binder>
    void def(Binder & binder)
    {
        // ...
    }
};
```

end::binder_module[] */
template <class Module>
struct module_
{
    // Alias shorthand for `b2::bind::type_` to avoid namespace qualification.
    template <class T>
    using type_ = ::b2::bind::type_<T>;

    // Alias shorthand for `b2::bind::init_` to avoid namespace qualification.
    template <class... A>
    using init_ = ::b2::bind::init_<A...>;

    // Get the sub-typed, from CRTP, `*this` reference.
    Module &self() { return *static_cast<Module *>(this); }
};

/** tag::binder_class[]

== `b2::bind::class_`

The `class_` type provides for defining class level members, i.e. methods. The
`class_` type is returned by the `binder_::def_class(..)` to give a quick, and
scoped, manner to define complete class bindings.
For example:

```
binder.def_class("system_info", type_<b2::system_info>())
    .def(init_<>())
    .def("cpu_core_count", &b2::system_info::cpu_core_count)
    .def("cpu_thread_count", &b2::system_info::cpu_thread_count);
```

end::binder_class[] */
template <class Class, class Binder>
struct class_
{
    const char *name = nullptr;
    Binder &binder;

    class_(const char *name, Binder &binder)
        : name(name), binder(binder)
    {
    }

    /** tag::binder_class[]

    === `b2::bind::class_::def(init_<Args...> init_args) -> class_ &`

    Defines an init method, i.e. constructor, with the given `init_args` types.
    The given `init_args` will match an appropriate constructor. And the
    constructor will be called from the bound language as appropriate for the
    language. For example for `jam` it will call the `__init__` rule.

    end::binder_class[] */
    template <class... Args>
    class_ &def(init_<Args...> init_args)
    {
        // Forward to the language specific binder.
        binder.def_init(this->name, (Class *)nullptr, init_args);
        return *this;
    }

    /** tag::binder_class[]

    === `b2::bind::class_::def(const char *name, F function) -> class_ &`

    Defines a method which it bound with the given `name` which calls the given
    `function`.

    end::binder_class[] */
    template <class F>
    class_ &def(const char *name, F function)
    {
        // Forward to the language specific binder.
        binder.def_method(this->name, name, function);
        return *this;
    }
};

/* tag::binder[]

Interface that converts between binder values and C++ values.

end::binder[] */
template <class Binder, class CxxValue, class BindValue>
struct converter_
{
    static BindValue to_bind_value(const CxxValue &);
    static CxxValue from_bind_value(BindValue);
};

/** tag::binder_binder[]

== `b2::bind::binder_`

Interface for a language agnostic binder. This is passed to the module
definition function (`template <class B> def(B & binder)`). What is passed
is a language specific subclass that will generate the needed bindings for
that language.

end::binder_binder[] */
template <class Binder>
struct binder_
{
    /** tag::binder_binder[]

    === `b2::bind::binder_::def_class`

    Declares the definition of a class, given in the `type_` wrapper, in the
    module.

    end::binder_binder[] */
    template <class Class>
    class_<Class, Binder> def_class(const char *name, type_<Class> class_type)
    {
        self().bind_class(current_module_name, name, class_type);
        return class_<Class, Binder>(name, self());
    }

    /** tag::binder_binder[]

    === `b2::bind::binder_::def_class`

    === `b2::bind::binder_::def(const char *name, F function) -> binder_ &`

    Defines a function which is bound with the given `name` which calls the given
    `function`.

    end::binder_binder[] */
    template <class F>
    binder_ &def(const char *name, F function)
    {
        // Forward to the language specific binder.
        self().def_function(name, function);
        return *this;
    }

    // Internal..

    // Returns the subclass reference to this binder.
    Binder &self() { return *static_cast<Binder *>(this); }

    // Binds the given native module declarations. This calls the subclass'
    // `bind_module(module_name)` method to do any binding work for the module.
    // And then calls the `def(binder)` method on the module to run through the
    // definitions of the module for this binder.
    template <class Module>
    Binder &bind(Module m)
    {
        current_module_name = m.module_name;
        self().bind_module(current_module_name);
        m.def(self());
        return self();
    }

    // Respond to a method definition of a class. This calls the subclass
    // method `bind_method(module_name, class_name, method_name, function)`.
    template <class Function>
    void def_method(const char *class_name, const char *method_name, Function f)
    {
        self().bind_method(current_module_name, class_name, method_name, f);
    }

    // Respond to a constructor definition of a class. This calls the subclass
    // method `bind_init(module_name, class_name, class_nullptr, init)`.
    template <class Class, class Init>
    void def_init(const char *class_name, Class *c, Init i)
    {
        self().bind_init(current_module_name, class_name, c, i);
    }

    // Respond to a function definition of a module. This calls the subclass
    // method `bind_function(module_name, function_name, function)`.
    template <class Function>
    void def_function(const char *function_name, Function f)
    {
        self().bind_function(current_module_name, function_name, f);
    }

    // Generic, shim, to convert from a C++ value to a binding specific value.
    // Forwards to the `converter_` template specialization.
    template <class BindValue, class CxxValue>
    static BindValue convert_to_bind_value(const CxxValue &source)
    {
        return converter_<Binder, CxxValue, BindValue>::to_bind_value(source);
    }

    // Generic, shim, to convert from a binding specific value to a C++ value.
    // Forwards to the `converter_` template specialization.
    template <class CxxValue, class BindValue>
    static CxxValue convert_from_bind_value(BindValue source)
    // static CxxValue convert_from_bind_value(const BindValue &source)
    {
        return converter_<Binder, CxxValue, BindValue>::from_bind_value(source);
    }

protected:
    const char *current_module_name = nullptr;
};

}; // namespace bind

// Bind B2 API to jam interpreter.
void bind_jam();

} // namespace b2

#endif
