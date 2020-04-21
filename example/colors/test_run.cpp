//
// Copyright (c) 2020 Mateusz Loskot <mateusz@loskot.net>
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/core/lightweight_test.hpp>

int sum(int a, int b) { return a + b; }

int main()
{
    BOOST_TEST_EQ(sum(2, 3), 5);
    return boost::report_errors();
}
