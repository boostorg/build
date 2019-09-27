/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_MP_H
#define B2_MP_H

namespace b2
{
namespace mp
{

// Simulate C++14 (make_)index_sequence..

template <std::size_t... Ns>
struct index_sequence
{
    using type = index_sequence;
};

template <typename A, typename B>
struct merge_index_sequence;
template <std::size_t... A, std::size_t... B>
struct merge_index_sequence<
    index_sequence<A...>, index_sequence<B...>>
    : index_sequence<A..., (sizeof...(A)+B)...>
{
};

template <std::size_t N>
struct make_index_sequence
    : merge_index_sequence<
        typename make_index_sequence<N/2>::type,
        typename make_index_sequence<N-(N/2)>::type>
{
};
template <> struct make_index_sequence<0> : index_sequence<>
{
};
template <> struct make_index_sequence<1> : index_sequence<0>
{
};


// Shallow, simulated, invoke..

template <typename Return, typename Call, typename ArgsTuple, std::size_t... I>
Return invoke(Call & call, ArgsTuple && args, index_sequence<I...>)
{
    return call(std::get<I>(args)...);
}


// Simulate C++ 20 remove_cvref..

template <typename T>
struct remove_cvref
{
    using type
        = typename std::remove_cv<
            typename std::remove_reference<T>::type>::type;
};


}
}

#endif
