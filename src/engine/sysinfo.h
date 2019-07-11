/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_SYSINFO_H
#define B2_SYSINFO_H

#include "config.h"
#include "bind.h"

namespace b2
{

/*
Provides information about the system, hardware and software, we are
running in.
*/
class system_info
{
public:
    system_info();

    /*
    Returns the number of physical CPU cores if available. Otherwise
    returns 1.

    Currently implemented for: OS_MACOSX.
    */
    unsigned int cpu_core_count();

    /*
    Returns the number of logical CPU threads is available. Otherwise
    returns `spu_core_count()`.

    Currently implemented for: OS_MACOSX.
    */
    unsigned int cpu_thread_count();

private:
    unsigned int cpu_core_count_ = 0;
    unsigned int cpu_thread_count_ = 0;
};

struct sysinfo_module
    : b2::bind::module_<sysinfo_module>
{
    const char *module_name = "sysinfo";

    template <class Binder>
    void def(Binder & binder)
    {
        binder.def_class("system_info", type_<b2::system_info>())
            .def(init_<>())
            .def("cpu_core_count", &b2::system_info::cpu_core_count)
            .def("cpu_thread_count", &b2::system_info::cpu_thread_count);
    }
};

} // namespace b2

#endif
