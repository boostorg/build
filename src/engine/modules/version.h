/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_VERSION_H
#define B2_VERSION_H

#include "../config.h"
#include "../bind.h"
#include <vector>

namespace b2
{
bool version_less(const std::vector<int> &lhs, const std::vector<int> &rhs);

struct version_module
    : b2::bind::module_<version_module>
{
    const char *module_name = "version";

    template <class Binder>
    void def(Binder& binder)
    {
        binder
            .def("version-less", &b2::version_less);
    }
};
}

#endif
