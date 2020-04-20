/*
    Copyright 2020 Mateusz Loskot <mateusz at loskot dot net>
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BJAM_COLOR_H
#define BJAM_COLOR_H

#include "lists.h"

enum TERMINAL_COLOR
{
    TC_DEFAULT = 0,

    TC_FOREGROUND_BLACK = 0x1,
    TC_FOREGROUND_RED = 0x2,
    TC_FOREGROUND_GREEN = 0x3,
    TC_FOREGROUND_YELLOW = 0x4,
    TC_FOREGROUND_BLUE = 0x5,
    TC_FOREGROUND_MAGENTA = 0x6,
    TC_FOREGROUND_CYAN = 0x7,
    TC_FOREGROUND_WHITE = 0x8,
    TC_FOREGROUND_MASK = 0xF,

    TC_BACKGROUND_BLACK = 0x10,
    TC_BACKGROUND_RED = 0x20,
    TC_BACKGROUND_GREEN = 0x30,
    TC_BACKGROUND_YELLOW = 0x40,
    TC_BACKGROUND_BLUE = 0x50,
    TC_BACKGROUND_MAGENTA = 0x60,
    TC_BACKGROUND_CYAN = 0x70,
    TC_BACKGROUND_WHITE = 0x80
};


enum TERMINAL_COLOR_STYLE
{
    TC_FOREGROUND_BRIGHT= 0x100,
    TC_BACKGROUND_BRIGHT= 0x200,
};

// Semantic colors for specific type of output
enum TERMINAL_COLOR_SEMANTIC
{
    TC_ACTION_NAME = TC_FOREGROUND_GREEN | TC_FOREGROUND_BRIGHT,
    TC_ACTION_TARGET = TC_DEFAULT,
    TC_ACTION_COMMAND = TC_FOREGROUND_YELLOW,
    TC_ERROR = TC_FOREGROUND_RED | TC_FOREGROUND_BRIGHT,
    TC_INFO = TC_FOREGROUND_YELLOW | TC_FOREGROUND_BRIGHT,
    TC_WARNING =  TC_FOREGROUND_MAGENTA | TC_FOREGROUND_BRIGHT
};

void set_color(FILE* bjam_stream, LIST * const colors);
void set_color(FILE* bjam_stream, char const* foreground_name, char const* background_name);
void set_color(FILE* bjam_stream, unsigned int colors);
void set_color_default(FILE* bjam_stream);

#endif
