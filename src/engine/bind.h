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

template <class Module>
struct module_
{
    template <class T>
    using type_ = ::b2::bind::type_<T>;

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
        m.def(self());
        return self();
    }

    template <class Class>
    detail::class_<Class, Binder> def_class(const char *name, bind::type_<Class>)
    {
        return detail::class_<Class, Binder>(name, self());
    }

    template <class Function>
    void def_method(const char *class_name, const char *method_name, Function f)
    {
        self().bind_method(current_module_name, class_name, method_name, f);
    }

protected:
    const char *current_module_name = nullptr;

    // template <class Function>
    // void bind_method(const char *module_name, const char *class_name, const char *method_name, Function f);
};

}; // namespace bind
} // namespace b2

#endif
