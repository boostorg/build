/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "bindjam.h"
#include "bind.h"
#include "native.h"

#include "sysinfo.h"

namespace b2
{

struct jam_binder : bind::binder_<jam_binder>
{
    template <class Function>
    void bind_method(
        const char *module_name, const char *class_name,
        const char *method_name, Function f)
    {
    }
};

void bind_jam()
{
    jam_binder()
        .bind(sysinfo_module());
}

} // namespace b2
