/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "sysinfo.h"
#include "jam.h"
#include "output.h"

#if defined(OS_MACOSX)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#if !defined(OS_NT)
#include <unistd.h>
#endif


b2::system_info::system_info()
{
}

unsigned int b2::system_info::cpu_core_count()
{
    if (cpu_core_count_ == 0)
    {
        #if defined(OS_MACOSX)
        int out_hw_ncpu = 0;
        size_t len_hw_ncpu = sizeof(out_hw_ncpu);
        int result = ::sysctlbyname(
            "hw.physicalcpu", &out_hw_ncpu, &len_hw_ncpu, nullptr, 0);
        if (result == 0)
        {
            cpu_core_count_ = out_hw_ncpu;
        }
        #elif defined(_SC_NPROCESSORS_CONF)
        cpu_thread_count_ = ::sysconf(_SC_NPROCESSORS_CONF);
        #endif
        if (cpu_core_count_ <= 0)
        {
            cpu_core_count_ = 1;
        }
    }
    return cpu_core_count_;
}

unsigned int b2::system_info::cpu_thread_count()
{
    if (cpu_thread_count_ == 0)
    {
        #if defined(OS_MACOSX)
        int out_hw_ncpu = 0;
        size_t len_hw_ncpu = sizeof(out_hw_ncpu);
        int result = ::sysctlbyname(
            "hw.logicalcpu", &out_hw_ncpu, &len_hw_ncpu, nullptr, 0);
        if (result == 0)
        {
            cpu_thread_count_ = out_hw_ncpu;
        }
        #elif defined(_SC_NPROCESSORS_ONLN)
        cpu_thread_count_ = ::sysconf(_SC_NPROCESSORS_ONLN);
        #endif
        if (cpu_thread_count_ <= 0)
        {
            cpu_thread_count_ = cpu_core_count();
        }
    }
    return cpu_thread_count_;
}
