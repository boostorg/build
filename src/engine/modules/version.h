/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#ifndef B2_VERSION_H
#define B2_VERSION_H

#include "../config.h"
#include "../bind.h"
#include <vector>

namespace b2
{
/* tag::version[]

= `version` module.

end::version[] */

/* tag::version[]

== `version_less`

Returns `true` if the first version, `lhs`, is semantically less than the
second version, `rhs`.

Jam::
`rule version-less ( lhs + : rhs + )`

end::version[] */
bool version_less(const std::vector<int> &lhs, const std::vector<int> &rhs);

struct version_module
    : b2::bind::module_<version_module>
{
    const char *module_name = "version";

    template <class Binder>
    void def(Binder& binder)
    {
        binder
            .def(&b2::version_less, "version-less", "lhs"*_1n | "rhs"*_1n);
    }
};
}

#endif
