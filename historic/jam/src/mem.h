/*
Copyright Rene Rivera 2006.
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BJAM_MEM_H
#define BJAM_MEM_H


#ifdef OPT_BOEHM_GC

    /* Use Boehm GC memory allocator. */
    #include <gc.h>
    #define bjam_malloc_x(s) GC_malloc(s)
    #define bjam_malloc_atomic_x(s) GC_malloc_atomic(s)
    #define bjam_calloc_x(n,s) memset(GC_malloc((n)*(s)),0,(n)*(s))
    #define bjam_calloc_atomic_x(n,s) memset(GC_malloc_atomic((n)*(s)),0,(n)*(s))
    #define bjam_realloc_x(p,s) GC_realloc(p,s)
    #define bjam_free_x(p) GC_free(p)
    #define bjam_mem_init_x() GC_init()

#else

    /* Standard C memory allocation. */
    #define bjam_malloc_x(s) malloc(s)
    #define bjam_calloc_x(n,s) calloc(n,s)
    #define bjam_realloc_x(p,s) realloc(p,s)
    #define bjam_free_x(p) free(p)

#endif

#ifndef bjam_malloc_atomic_x
    #define bjam_malloc_atomic_x(s) bjam_malloc_x(s)
#endif
#ifndef bjam_calloc_atomic_x
    #define bjam_calloc_atomic_x(s) bjam_calloc_x(s)
#endif
#ifndef bjam_mem_init_x
    #define bjam_mem_init_x()
#endif
#ifndef bjam_mem_close_x
    #define bjam_mem_close_x()
#endif

#ifdef OPT_DEBUG_PROFILE

    /* Profile tracing of memory allocations. */
    #define BJAM_MALLOC(s) (profile_memory(s), bjam_malloc_x(s))
    #define BJAM_MALLOC_ATOMIC(s) (profile_memory(s), bjam_malloc_atomic_x(s))
    #define BJAM_CALLOC(n,s) (profile_memory(n*s), bjam_calloc_x(n,s))
    #define BJAM_CALLOC_ATOMIC(n,s) (profile_memory(n*s), bjam_calloc_atomic_x(n,s))
    #define BJAM_REALLOC(p,s) (profile_memory(s), bjam_realloc_x(p,s))
    #define BJAM_FREE(p) bjam_free_x(p)
    #define BJAM_MEM_INIT() bjam_mem_init_x()
    #define BJAM_MEM_CLOSE() bjam_mem_close_x()

#else

    /* No mem tracing. */
    #define BJAM_MALLOC(s) bjam_malloc_x(s)
    #define BJAM_MALLOC_ATOMIC(s) bjam_malloc_atomic_x(s)
    #define BJAM_CALLOC(n,s) bjam_calloc_x(n,s)
    #define BJAM_CALLOC_ATOMIC(n,s) bjam_calloc_atomic_x(n,s)
    #define BJAM_REALLOC(p,s) bjam_realloc_x(p,s)
    #define BJAM_FREE(p) bjam_free_x(p)
    #define BJAM_MEM_INIT() bjam_mem_init_x()
    #define BJAM_MEM_CLOSE() bjam_mem_close_x()

#endif


#endif
