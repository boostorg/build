//
// Copyright (c) 2020 Mateusz Loskot <mateusz@loskot.net>
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>

struct A {};

int main()
{
    static_assert(std::is_same<A, A>::value, "A != A");
}
