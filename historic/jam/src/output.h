/* 
    Copyright 2007 Rene Rivera
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BJAM_OUTPUT_H
#define BJAM_OUTPUT_H

void out_action(
    const char * action,
    const char * target,
    const char * command,
    const char * out_data,
    const char * err_data
    );

#endif
