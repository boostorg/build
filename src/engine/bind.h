/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_BIND_H
#define B2_BIND_H

#include "config.h"
#include <functional>

namespace b2
{
namespace bind
{

template <typename T>
struct type_
{
    typedef T type;
};

template <typename... Args>
struct init_
{
};

template <class Module>
struct module_
{
    template <class T>
    using type_ = ::b2::bind::type_<T>;

    template <class... A>
    using init_ = ::b2::bind::init_<A...>;

    module_()
    {
    }

    Module &self() { return *static_cast<Module *>(this); }
};

namespace detail
{
template <class Class, class Binder>
struct class_
{
    const char *name = nullptr;
    Binder &binder;

    class_(const char *name, Binder &binder)
        : name(name), binder(binder)
    {
    }

    template <class F>
    class_ &def(const char *name, F function)
    {
        binder.def_method(this->name, name, function);
        return *this;
    }

    template <class... Args>
    class_ &def(init_<Args...> i)
    {
        binder.def_init(this->name, (Class*) nullptr, i);
        return *this;
    }
};
} // namespace detail

template <class Binder>
struct binder_
{
    Binder &self() { return *static_cast<Binder *>(this); }

    template <class Module>
    Binder &bind(Module m)
    {
        current_module_name = m.module_name;
        self().bind_module(current_module_name);
        m.def(self());
        return self();
    }

    template <class Class>
    detail::class_<Class, Binder> def_class(const char *name, bind::type_<Class> class_type)
    {
        self().bind_class(current_module_name, name, class_type);
        return detail::class_<Class, Binder>(name, self());
    }

    template <class Function>
    void def_method(const char *class_name, const char *method_name, Function f)
    {
        self().bind_method(current_module_name, class_name, method_name, f);
    }

    template <class Class, class Init>
    void def_init(const char *class_name, Class * c, Init i)
    {
        self().bind_init(current_module_name, class_name, c, i);
    }

protected:
    const char *current_module_name = nullptr;

    // template <class Function>
    // void bind_method(const char *module_name, const char *class_name, const char *method_name, Function f);
};

}; // namespace bind

// Bind B2 API to jam interpreter.
void bind_jam();

} // namespace b2

#endif
