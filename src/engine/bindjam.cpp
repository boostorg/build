/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "bindjam.h"
#include "class.h"
#include "function.h"
#include "hash.h"
#include "lists.h"
#include "modules.h"
#include "mp.h"
#include "native.h"
#include "optval.h"
#include "output.h"
#include "rules.h"
#include "variable.h"
#include "types.h"

#include "modules/mod_regex.h"
#include "modules/mod_set.h"
#include "modules/mod_string.h"
#include "modules/sysinfo.h"
#include "modules/version.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

namespace b2
{

/*
Basic core types to marshal..
*/

template <>
string_t from_jam<string_t>(OBJECT *jv)
{
    return object_str(jv);
}
template <>
OBJECT *to_jam(string_t v)
{
    return object_new(v.c_str());
}

template <>
uint_t from_jam<uint_t>(OBJECT *jv)
{
    return std::stoul(from_jam<string_t>(jv));
}
template <>
OBJECT *to_jam(uint_t v)
{
    return to_jam(std::to_string(v));
}

template <>
int_t from_jam<int_t>(OBJECT *jv)
{
    return std::stoi(from_jam<string_t>(jv));
}
template <>
OBJECT *to_jam(int_t v)
{
    return to_jam(std::to_string(v));
}

template <>
bool from_jam<bool>(OBJECT *jv)
{
    return object_str(jv) != nullptr && object_str(jv)[0] != '\0';
}
template <>
OBJECT *to_jam(bool v)
{
    return object_new(v ? "1" : "");
}

namespace bind
{
// General marshaling of one jam value list. Default converts the first item
// to/from the list.
template <typename CxxValue>
struct converter_<jam_binder, CxxValue, LIST *>
{
    static LIST *to_bind_value(const CxxValue &cpp_value)
    {
        return list_new(to_jam(cpp_value));
    }
    static CxxValue from_bind_value(LIST *jam_value)
    {
        return from_jam<CxxValue>(list_front(jam_value));
    }
};

// Marshaling specialization for vector container.
// TODO: Generalize to more containers.
template <typename Value>
struct converter_<jam_binder, std::vector<Value>, LIST *>
{
    static LIST *to_bind_value(const std::vector<Value> &cpp_value)
    {
        LIST *result = L0;
        for (auto &v : cpp_value)
        {
            result = list_push_back(result, to_jam(v));
        }
        return result;
    }
    static std::vector<Value> from_bind_value(LIST *jam_value)
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

// Marshaling for optval container.
template <typename ValueType>
struct converter_<jam_binder, optval<ValueType>, LIST *>
{
    static LIST *to_bind_value(const optval<ValueType> &cpp_value)
    {
        LIST *result = L0;
        if (cpp_value.has_value())
        {
            result = list_push_back(result, to_jam(static_cast<ValueType>(cpp_value)));
        }
        return result;
    }
    static optval<ValueType> from_bind_value(LIST *jam_value)
    {
        if (list_length(jam_value) == 0)
            return optval<ValueType>{};
        return from_jam<ValueType>(list_front(jam_value));
    }
};

// Marshaling of tuples as multi-param arguments.
template <typename... ValueTypes>
struct converter_<jam_binder, std::tuple<ValueTypes...>, LIST *>
{
    using TupleType = std::tuple<ValueTypes...>;
    template <std::size_t... I>
    static LIST *to_bind_value(LIST *result, const TupleType &cpp_value, mp::index_sequence<I...>)
    {
        using std::get;
        OBJECT *jam_val[]{nullptr, to_jam(get<I>(cpp_value))...};
        for (int i = 1; i <= std::tuple_size<TupleType>::value; ++i)
        {
            result = list_push_back(result, jam_val[i]);
        }
        return result;
    }
    static LIST *to_bind_value(const TupleType &cpp_value)
    {
        return to_bind_value(L0, cpp_value, mp::make_index_sequence<std::tuple_size<TupleType>::value>{});
    }
    template <std::size_t... I>
    static TupleType from_bind_value(LIST *jam_value, mp::index_sequence<I...>)
    {
        TupleType result;
        LISTITER jam_value_i = list_begin(jam_value);
        LISTITER jam_value_e = list_end(jam_value);
        using K = int[];
        (void)K{0, ((void)(std::get<I>(result) = from_jam<typename std::tuple_element<I, TupleType>::type>(jam_value_i[I])), 0)...};
        return result;
    }
    static TupleType from_bind_value(LIST *jam_value)
    {
        return from_bind_value(jam_value, mp::make_index_sequence<std::tuple_size<TupleType>::value>{});
    }
};

} // namespace bind

struct jam_cxx_self
{
    // Get the hidden C++ self instance for the jam class instance.
    template <typename Class>
    static Class *get(FRAME *frame)
    {
        OBJECT *cxx_self_name = object_new("__cxx_self__");
        const char *cxx_self_value = object_str(list_front(var_get(frame->module, cxx_self_name)));
        object_free(cxx_self_name);
        std::intptr_t result = 0;
        for (int i = sizeof(void *) - 1; i >= 0; --i)
        {
            result = (result << 8) + (cxx_self_value[i * 2 + 0] - 'a') +
                     ((cxx_self_value[i * 2 + 1] - 'a') << 4);
        }
        Class *self = reinterpret_cast<Class *>(result);
        return self;
    }

    // Set the hidden C++ self instance for the jam class instance.
    template <typename Class>
    static void set(FRAME *frame, Class *self)
    {
        // We hide the instance in a string value by appending the raw
        // pointer past the end of the string.
        // jam_cxx_self cxx_self_value {"CXX", self};
        char cxx_self_value[sizeof(void *) * 2 + 1];
        std::intptr_t self_int = reinterpret_cast<std::intptr_t>(self);
        for (int i = 0; i < sizeof(void *); ++i)
        {
            cxx_self_value[i * 2 + 0] = char(self_int & 0xf) + 'a';
            cxx_self_value[i * 2 + 1] = char((self_int & 0xf0) >> 4) + 'a';
            self_int >>= 8;
        }
        cxx_self_value[sizeof(void *) * 2] = 0;
        OBJECT *cxx_self_name = object_new("__cxx_self__");
        OBJECT *cxx_self_obj = object_new(cxx_self_value);
        var_set(
            frame->module, cxx_self_name,
            list_new(cxx_self_obj),
            VAR_SET);
        object_free(cxx_self_name);
    }
};

// Marshals arguments from Jam LOL to C++ tuple..

template <typename ArgsTuple, std::size_t N, typename Enable = void>
struct jam_marshal_args
{
    static void convert(ArgsTuple &to_args, LOL *from_args)
    {
    }
};
template <typename ArgsTuple, std::size_t N>
struct jam_marshal_args<ArgsTuple, N,
                        typename std::enable_if<(N < std::tuple_size<ArgsTuple>::value)>::type>
{
    static void convert(ArgsTuple &to_args, LOL *from_args)
    {
        using arg_type = typename std::tuple_element<N, ArgsTuple>::type;
        LIST *arg_value = lol_get(from_args, N);
        if (arg_value != L0)
            std::get<N>(to_args) = jam_binder::convert_from_bind_value<arg_type>(arg_value);

        if (N < std::tuple_size<ArgsTuple>::value)
        {
            jam_marshal_args<ArgsTuple, N + 1>::convert(to_args, from_args);
        }
    }
};

// Bound init/constructor function that forwards from jam __init__ to C++.
template <typename Call, typename Class, typename... Args>
static LIST *jam_call_init(
    FRAME *frame, int flags,
    Call cxx_call,
    Class *(*)(Args...))
{
    using namespace mp;
    try
    {
        // Marshal arguments from frame->args.
        using ArgsTuple = std::tuple<typename remove_cvref<Args>::type...>;
        ArgsTuple args;
        jam_marshal_args<ArgsTuple, 0>::convert(args, frame->args);
        // Construct the class instance, and set the hidden jam instance var
        // to keep track of it.
        Class *self = invoke<Class *>(
            cxx_call,
            args,
            make_index_sequence<std::tuple_size<ArgsTuple>::value>{});
        jam_cxx_self::set(frame, self);
        // Nothing to return from an init.
        return L0;
    }
    catch (const std::exception &e)
    {
        return L0;
    }
}

// Bound plain function that forwards from jam to C++ with a return
// converted back to jam.
template <
    typename Call, typename Class, typename... Args, typename Return,
    typename std::enable_if<!std::is_void<Return>::value, int>::type = 0>
static LIST *jam_call_method(
    FRAME *frame, int flags,
    Call cxx_call,
    Return (*)(Class *, Args...))
{
    using namespace mp;
    try
    {
        // Marshal arguments from frame->args.
        using ArgsTuple = std::tuple<typename remove_cvref<Args>::type...>;
        ArgsTuple args;
        jam_marshal_args<ArgsTuple, 0>::convert(args, frame->args);
        // Invoke call, and return marshaled result.
        return jam_binder::convert_to_bind_value<LIST *>(
            invoke<Return>(
                cxx_call,
                std::tuple_cat(
                    std::make_tuple(jam_cxx_self::get<Class>(frame)),
                    args),
                make_index_sequence<1 + std::tuple_size<ArgsTuple>::value>{}));
    }
    catch (const std::exception &e)
    {
        return L0;
    }
}

// Bound plain function that forwards from jam to C++ with a void return.
template <
    typename Call, typename Class, typename... Args, typename Return,
    typename std::enable_if<std::is_void<Return>::value, int>::type = 0>
static LIST *jam_call_method(
    FRAME *frame, int flags,
    Call cxx_call,
    Return (*)(Class *, Args...))
{
    using namespace mp;
    try
    {
        // Marshal arguments from frame->args.
        using ArgsTuple = std::tuple<typename remove_cvref<Args>::type...>;
        ArgsTuple args;
        jam_marshal_args<ArgsTuple, 0>::convert(args, frame->args);
        // Invoke call, and return marshaled result.
        invoke<Return>(
            cxx_call,
            std::tuple_cat(
                std::make_tuple(jam_cxx_self::get<Class>(frame)),
                args),
            make_index_sequence<1 + std::tuple_size<ArgsTuple>::value>{});
        return L0;
    }
    catch (const std::exception &e)
    {
        return L0;
    }
}

// Bound plain function...
template <
    typename Call, typename... Args, typename Return,
    typename std::enable_if<!std::is_void<Return>::value, int>::type = 0>
static LIST *jam_call_function(
    FRAME *frame, int flags,
    Call cxx_call,
    Return (*)(Args...))
{
    using namespace mp;
    try
    {
        // Marshal arguments from frame->args.
        using ArgsTuple = std::tuple<typename remove_cvref<Args>::type...>;
        ArgsTuple args;
        jam_marshal_args<ArgsTuple, 0>::convert(args, frame->args);
        // Invoke call, and return marshaled result.
        return jam_binder::convert_to_bind_value<LIST *>(
            invoke<Return>(
                cxx_call, args,
                make_index_sequence<std::tuple_size<ArgsTuple>::value>{}));
    }
    catch (const std::exception &e)
    {
        return L0;
    }
}
template <
    typename Call, typename... Args, typename Return,
    typename std::enable_if<std::is_void<Return>::value, int>::type = 0>
static LIST *jam_call_function(
    FRAME *frame, int flags,
    Call cxx_call,
    Return (*)(Args...))
{
    using namespace mp;
    try
    {
        // Marshal arguments from frame->args.
        using ArgsTuple = std::tuple<typename remove_cvref<Args>::type...>;
        ArgsTuple args;
        jam_marshal_args<ArgsTuple, 0>::convert(args, frame->args);
        // Invoke call, and return marshaled result.
        invoke<Return>(
            cxx_call, args,
            make_index_sequence<std::tuple_size<ArgsTuple>::value>{});
        return L0;
    }
    catch (const std::exception &e)
    {
        return L0;
    }
}

template <typename Return, typename... Args>
void jam_native_bind(
    const string_t &module_name,
    const string_t &rule_name,
    function_builtin_t native_rule,
    Return (*)(Args...))
{
    OBJECT *module_name_obj = object_new(module_name.c_str());
    OBJECT *rule_name_obj = object_new(rule_name.c_str());
    module_t *module = bindmodule(module_name_obj);

    int found = 0;
    RULE *const rule = (RULE *)hash_insert(demand_rules(module), rule_name_obj, &found);
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
    native_rule_t *np = (native_rule_t *)hash_find(module->native_rules, rule_name_obj);
    // Set a global name for the native rule.
    function_set_rulename(
        np->procedure,
        object_new((module_name + "." + rule_name).c_str()));
    // Define the native rule in the class module.
    new_rule_body(module, np->name, np->procedure, 1);

    object_free(module_name_obj);
    object_free(rule_name_obj);
}

template <typename Return, typename Class, typename... Args>
void jam_bind(
    const string_t &module_name,
    const string_t &rule_name,
    Return (Class::*method)(Args...))
{
    // out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [method](FRAME *frame, int flags) -> LIST * {
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
    const string_t &module_name,
    const string_t &rule_name,
    Return (Class::*method)(Args...) const)
{
    // out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [method](FRAME *frame, int flags) -> LIST * {
            return jam_call_method(
                frame, flags,
                [method](const Class *self, Args... args) -> Return {
                    return (self->*method)(args...);
                },
                static_cast<Return (*)(Class *, Args...)>(nullptr));
        },
        static_cast<Return (*)(Class *, Args...)>(nullptr));
}

template <typename Return, typename... Args>
void jam_bind(
    const string_t &module_name,
    const string_t &rule_name,
    Return (*function)(Args...))
{
    jam_native_bind(
        module_name, rule_name,
        [function](FRAME *frame, int flags) -> LIST * {
            return jam_call_function(
                frame, flags,
                [function](Args... args) -> Return {
                    return (*function)(args...);
                },
                static_cast<Return (*)(Args...)>(nullptr));
        },
        static_cast<Return (*)(Args...)>(nullptr));
}

template <typename Class, typename... Args>
void jam_bind(
    const string_t &module_name,
    const string_t &rule_name,
    Class *,
    ::b2::bind::init_<Args...>)
{
    // out_printf( "jam_bind: %s.%s.\n", module_name.c_str(), rule_name.c_str() );
    jam_native_bind(
        module_name, rule_name,
        [module_name, rule_name](FRAME *frame, int flags) -> LIST * {
            return jam_call_init(
                frame, flags,
                [](Args... args) -> Class * {
                    return new Class(args...);
                },
                static_cast<Class *(*)(Args...)>(nullptr));
        },
        (void (*)(Args...)) nullptr);
}

void jam_binder::bind_module(
    const char *module_name)
{
    OBJECT *module_name_obj = object_new(module_name);
    bindmodule(module_name_obj);
    object_free(module_name_obj);
}

template <typename Class, typename... Bases>
void jam_binder::bind_class(
    const char *module_name, const char *class_name,
    ::b2::bind::type_<Class>,
    ::b2::bind::type_<Bases>...)
{
    OBJECT *class_name_obj = object_new(class_name);
    LIST *class_name_list = list_new(class_name_obj);
    OBJECT *bases_name_obj[] = {
        object_new(::b2::bind::class_<Bases, jam_binder>::name())..., nullptr
    };
    LIST *bases_name_list = L0;
    for (OBJECT *base_name_obj : bases_name_obj)
    {
        if (base_name_obj)
            bases_name_list = list_push_back(bases_name_list, base_name_obj);
    }
    /* OBJECT * class_module = */ make_class_module(class_name_list, bases_name_list);
    object_free(class_name_obj);
    for (OBJECT *base_name_obj : bases_name_obj)
    {
        if (base_name_obj)
            object_free(base_name_obj);
    }
    list_free(bases_name_list);
}

template <typename Function>
void jam_binder::bind_method(
    const char *module_name, const char *class_name,
    const char *method_name, Function f)
{
    jam_bind(string_t("class@") + class_name, method_name, f);
}

template <typename Class, typename Init>
void jam_binder::bind_init(
    const char *module_name, const char *class_name,
    Class *c, Init i)
{
    jam_bind(string_t("class@") + class_name, "__init__", c, i);
}

template <typename Function>
void jam_binder::bind_function(
    const char *module_name,
    const char *function_name, Function f)
{
    jam_bind(module_name, function_name, f);
}

void bind_jam()
{
    jam_binder()
        .bind(sysinfo_module())
        .bind(version_module())
        .bind(string_module())
        .bind(set_module())
        .bind(regex_module());
}

} // namespace b2
