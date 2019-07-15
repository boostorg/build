/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "bindjam.h"
#include "bind.h"
#include "class.h"
#include "function.h"
#include "hash.h"
#include "lists.h"
#include "modules.h"
#include "native.h"
#include "object.h"
#include "output.h"
#include "rules.h"
#include "variable.h"

#include "sysinfo.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

namespace b2
{

// Marshaling of arguments and results end up calling one of from_jam or
// to_jam functions. These are specialized on some core types as needed.

template <class Value> Value from_jam(OBJECT * jam_value);
template <class Value> OBJECT * to_jam(Value value);

template <> std::string from_jam<std::string>(OBJECT * jv)
{
    return object_str(jv);
}
template <> OBJECT * to_jam(std::string v)
{
    return object_new(v.c_str());
}

template <> unsigned int from_jam<unsigned int>(OBJECT * jv)
{
    return std::stoul(from_jam<std::string>(jv));
}
template <> OBJECT * to_jam(unsigned int v)
{
    return to_jam(std::to_string(v));
}

// Utility to get the first type in a parameter pack.
template <class... T>
struct pack_front
{
    using type = typename std::tuple_element<0, std::tuple<T...>>::type;
};
template <class... T>
using pack_front_t = typename pack_front<T...>::type;

// General marshaling of one jam value list. Default converts the first item
// to/from the list.
template <typename Value>
struct jam_marshal
{
    static LIST * convert(Value cpp_value)
    {
        return list_new(to_jam(cpp_value));
    }
    static Value convert(LIST * jam_value)
    {
        return from_jam<Value>(list_front(jam_value));
    }
};

// Specialize for void to reduce code duplication.
template <>
struct jam_marshal<void>
{
    static LIST * convert(void)
    {
        return L0;
    }
    static void convert(LIST * jam_value)
    {
    }
};

// Marshaling specialization for vector container.
// TODO: Generalize to more containers.
template <class Value>
struct jam_marshal<std::vector<Value>>
{
    static LIST * convert(const std::vector<Value> & cpp_value)
    {
        LIST * result = L0;
        for (auto & v: cpp_value)
        {
            list_append(to_jam(v));
        }
        return result;
    }
    static std::vector<Value> convert(LIST * jam_value)
    {
        std::vector<Value> result;
        const LISTITER e = list_end(jam_value);
        for (LISTITER i = list_begin(jam_value); i != e; i = list_next(i))
        {
            result.emplace_back(from_jam<Value>(list_item(i)));
        }
        return result;
    }
};

struct jam_cxx_self
{
    // Get the hidden C++ self instance for the jam class instance.
    template <typename Class>
    static Class * get(FRAME * frame)
    {
        OBJECT * cxx_self_name = object_new("__cxx_self__");
        const char * cxx_self_value
            = object_str(list_front(var_get(frame->module, cxx_self_name)));
        object_free(cxx_self_name);
        std::intptr_t result = 0;
        for (int i = sizeof(void*)-1; i >= 0; --i)
        {
            result = (result << 8) + (cxx_self_value[i*2+0] - 'a') +
                ((cxx_self_value[i*2+1] - 'a') << 4);
        }
        Class * self = reinterpret_cast<Class *>(result);
        return self;
    }

    // Set the hidden C++ self instance for the jam class instance.
    template <typename Class>
    static void set(FRAME * frame, Class * self)
    {
        // We hide the instance in a string value by appending the raw
        // pointer past the end of the string.
        // jam_cxx_self cxx_self_value {"CXX", self};
        char cxx_self_value[sizeof(void*)*2+1];
        std::intptr_t self_int = reinterpret_cast<std::intptr_t>(self);
        for (int i = 0; i < sizeof(void*); ++i)
        {
            cxx_self_value[i*2+0] = char(self_int & 0xf) + 'a';
            cxx_self_value[i*2+1] = char((self_int & 0xf0) >> 4) + 'a';
            self_int >>= 8;
        }
        cxx_self_value[sizeof(void*)*2] = 0;
        OBJECT * cxx_self_name = object_new("__cxx_self__");
        OBJECT * cxx_self_obj = object_new(cxx_self_value);
        var_set(
            frame->module, cxx_self_name,
            list_new(cxx_self_obj),
            VAR_SET);
        object_free(cxx_self_name);
    }
};

// Bound init/constructor function that forwards from jam __init__ to C++.
template <typename Call, typename Class, typename... Args>
static LIST * jam_call_init(
    FRAME * frame, int flags,
    Call cxx_call,
    Class * (*)(Args...))
{
    // Marshal arguments from frame->args.
    // ..
    // Construct the class instance, and set the hidden jam instance var
    // to keep track of it.
    Class * self = cxx_call();
    jam_cxx_self::set(frame, self);
    // Nothing to return from an init.
    return L0;
}

// Bound plain function that forwards from jam to C++ with a return
// converted back to jam.
template <typename Call, typename Class, typename... Args, typename Return>
static LIST * jam_call_method(
    FRAME * frame, int flags,
    Call cxx_call,
    Return (*)(Class *, Args...))
{
    // Marshal arguments from frame->args.
    // Invoke call.
    // .. Return r = call(..);
    Return r = cxx_call(jam_cxx_self::get<Class>(frame));
    // Marshal result to LIST.
    return jam_marshal<Return>::convert(r);
}


template <typename Return, typename... Args>
void jam_native_bind(
    const std::string & module_name,
    const std::string & rule_name,
    function_builtin_t native_rule,
    Return (*)(Args...))
{
    OBJECT * module_name_obj = object_new(module_name.c_str());
    OBJECT * rule_name_obj = object_new(rule_name.c_str());
    module_t * module = bindmodule(module_name_obj);

    int found = 0;
    RULE * const rule
        = (RULE *)hash_insert(demand_rules(module), rule_name_obj, &found);
    rule->name = object_copy(rule_name_obj);
    rule->procedure = 0;
    rule->module = module;
    rule->actions = 0;
    rule->exported = 0;
    // Register the function as a native jam rule.
    declare_native_rule(
        module_name.c_str(),
        rule_name.c_str(),
        nullptr, // TODO: Argument list.
        native_rule,
        0);
    // Note, we don't check results of not finding the existing native rule,
    // for the obvious reason that we just created it a few lines above.
    native_rule_t * np
        = (native_rule_t *)hash_find(module->native_rules, rule_name_obj );
    // Set a global name for the native rule.
    function_set_rulename(
        np->procedure,
        object_new((module_name+"."+rule_name).c_str()));
    // Define the native rule in the class module.
    new_rule_body(module, np->name, np->procedure, 1);

    object_free(module_name_obj);
    object_free(rule_name_obj);
}

// template <typename Return, typename Class, typename... Args>
// struct member_function
// {
//     typedef Return (Class::*pointer_t)(Args...);
// };

template <typename Return, typename Class, typename... Args>
void jam_bind(
    const std::string & module_name,
    const std::string & rule_name,
    Return (Class::*method)(Args...))
{
    out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [method](FRAME * frame, int flags) -> LIST *
        {
            return jam_call_method(
                frame, flags,
                [method](Class *self, Args... args) -> Return {
                    return (self->*method)(args...);
                },
                static_cast<Return (*)(Class *, Args...)>(nullptr));
        },
        static_cast<Return (*)(Class *, Args...)>(nullptr));
}

template <typename Return, typename Class, typename... Args>
void jam_bind(
    const std::string & module_name,
    const std::string & rule_name,
    Return (Class::*method)(Args...) const)
{
    out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [method](FRAME * frame, int flags) -> LIST *
        {
            return jam_call_method(
                frame, flags,
                [method](const Class *self, Args... args) -> Return {
                    return (self->*method)(args...);
                },
                static_cast<Return (*)(Class *, Args...)>(nullptr));
        },
        static_cast<Return (*)(Class *, Args...)>(nullptr));
}

template <class Class, typename... Args>
void jam_bind(
    const std::string & module_name,
    const std::string & rule_name,
    Class *,
    ::b2::bind::init_<Args...>)
{
    out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [module_name, rule_name](FRAME * frame, int flags) -> LIST *
        {
            return jam_call_init(
                frame, flags,
                [](Args... args) -> Class * {
                    return new Class(args...);
                },
                static_cast<Class * (*)(Args...)>(nullptr));
        },
        (void (*)(Args...)) nullptr);
}

struct jam_binder : bind::binder_<jam_binder>
{
    void bind_module(
        const char * module_name)
    {
        OBJECT * module_name_obj = object_new(module_name);
        bindmodule(module_name_obj);
        object_free(module_name_obj);
    }

    template <class Class>
    void bind_class(
        const char *module_name, const char *class_name,
        ::b2::bind::type_<Class>)
    {
        // Register the class "metaclass". Which in the case of jam is amodule
        // with the rules and variables that gets imported and copied into the
        // specific instance module as needed.
        OBJECT * class_name_obj = object_new(class_name);
        LIST * class_name_list = list_new(class_name_obj);
        OBJECT * class_module = make_class_module(class_name_list, L0);
        object_free(class_name_obj);
    }

    template <class Function>
    void bind_method(
        const char *module_name, const char *class_name,
        const char *method_name, Function f)
    {
        jam_bind(std::string("class@")+class_name, method_name, f);
    }

    template <class Class, class Init>
    void bind_init(
        const char *module_name, const char *class_name,
        Class * c, Init i)
    {
        jam_bind(std::string("class@")+class_name, "__init__", c, i);
    }

    std::vector<std::function<void(FRAME *)>> init_actions;
};

void bind_jam()
{
    jam_binder()
        .bind(sysinfo_module());
}

} // namespace b2
