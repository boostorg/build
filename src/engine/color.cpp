/*
    Copyright 2020 Mateusz Loskot <mateusz at loskot dot net>
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#include "jam.h"
#include "lists.h"
#include "output.h"
#include "color.h"

#include <map>
#include <string>

#if defined( OS_NT ) || defined( OS_CYGWIN )
#include <windows.h>
#else
#include <unistd.h>
#endif

enum TERMINAL_COLOR_MASK
{
    TC_MASK_FOREGROUND = 0x0F,
    TC_MASK_BACKGROUND = 0xF0
};

static unsigned int color_background(char const* color_name)
{
    if (!color_name)
        return TC_DEFAULT;

    static const std::map<std::string, unsigned int> names = {
        {"black", TC_BACKGROUND_BLACK},
        {"red", TC_BACKGROUND_RED},
        {"green", TC_BACKGROUND_GREEN},
        {"yellow", TC_BACKGROUND_YELLOW},
        {"blue", TC_BACKGROUND_BLUE},
        {"magenta", TC_BACKGROUND_MAGENTA},
        {"cyan", TC_BACKGROUND_CYAN},
        {"white", TC_BACKGROUND_WHITE}
    };

     unsigned int color = TC_DEFAULT;

    // Search for e.g. "red" or "red-bright" or "red-bold"
    std::string const name(color_name);
    std::size_t const style = name.find_first_of('-');
    if (style != std::string::npos && name[style + 1] == 'b') // bright
        color |= TC_BACKGROUND_BRIGHT;

    auto const it = names.find(name.substr(0, style));
    if (it != names.end())
        color |= it->second;
    else
        color = TC_DEFAULT;

    return color;
}

static unsigned int color_foreground(char const* color_name)
{
    if (!color_name)
        return TC_DEFAULT;

    static const std::map<std::string, unsigned int> names = {
        {"black", TC_FOREGROUND_BLACK},
        {"red", TC_FOREGROUND_RED},
        {"green", TC_FOREGROUND_GREEN},
        {"yellow", TC_FOREGROUND_YELLOW},
        {"blue", TC_FOREGROUND_BLUE},
        {"magenta", TC_FOREGROUND_MAGENTA},
        {"cyan", TC_FOREGROUND_CYAN},
        {"white", TC_FOREGROUND_WHITE},
        {"error", TC_ERROR},
        {"info", TC_INFO},
        {"warn", TC_WARNING},
        {"warning", TC_WARNING}
    };

    unsigned int color = TC_DEFAULT;

    // Search for e.g. "red" or "red-bright" or "red-bold"
    std::string const name(color_name);
    std::size_t const style = name.find_first_of('-');
    if (style != std::string::npos && name[style + 1] == 'b') // bright
        color |= TC_FOREGROUND_BRIGHT;

    auto const it = names.find(name.substr(0, style));
    if (it != names.end())
        color |= it->second;
    else
        color = TC_DEFAULT;

    return color;
}

void set_color(FILE* bjam_stream, LIST* const colors)
{
    if (!globs.colors)
        return;

    if (list_empty(colors))
        return;

    char const* foreground_color = nullptr;
    char const* background_color = nullptr;

    LISTITER iter = list_begin(colors);
    LISTITER const end = list_end(colors);

    if (iter != end)
        foreground_color = object_str(list_item(iter));
    if (!foreground_color)
        return;

    iter = list_next(iter);
    if (iter != end)
        background_color = object_str(list_item(iter));

    set_color(bjam_stream, foreground_color, background_color);
}

void set_color(FILE* bjam_stream, char const* foreground_name, char const* background_name)
{
    if (!globs.colors)
        return;

    unsigned int const f_color = color_foreground(foreground_name);
    unsigned int const b_color = color_background(background_name);

    set_color(bjam_stream, f_color | b_color);
}

void set_color(FILE* bjam_stream, unsigned int colors)
{
    if (!globs.colors)
        return;

#if defined( OS_NT ) || defined( OS_CYGWIN )
    static unsigned short const default_attr = []
    {
        // assume stdout and stderr share common defaults
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO buffer_info = {0};
        GetConsoleScreenBufferInfo(h, &buffer_info);
        return buffer_info.wAttributes;
    }();

    unsigned short attr = 0;
    switch (colors & TC_MASK_FOREGROUND)
    {
    case TC_DEFAULT:
        attr |= default_attr & (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case TC_FOREGROUND_BLACK:
        attr |= 0;
        break;
    case TC_FOREGROUND_RED:
        attr |= FOREGROUND_RED;
        break;
    case TC_FOREGROUND_GREEN:
        attr |= FOREGROUND_GREEN;
        break;
    case TC_FOREGROUND_YELLOW:
        attr |= FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    case TC_FOREGROUND_BLUE:
        attr |= FOREGROUND_BLUE;
        break;
    case TC_FOREGROUND_MAGENTA:
        attr |= FOREGROUND_RED | FOREGROUND_BLUE;
        break;
    case TC_FOREGROUND_CYAN:
        attr |= FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
    case TC_FOREGROUND_WHITE:
        attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
    }
    if (colors & TC_FOREGROUND_BRIGHT)
        attr |= FOREGROUND_INTENSITY;

    switch (colors & TC_MASK_BACKGROUND)
    {
    case TC_DEFAULT:
        attr |= default_attr & (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        break;
    case TC_BACKGROUND_BLACK:
        attr |= 0;
        break;
    case TC_BACKGROUND_RED:
        attr |= BACKGROUND_RED;
        break;
    case TC_BACKGROUND_GREEN:
        attr |= BACKGROUND_GREEN;
        break;
    case TC_BACKGROUND_YELLOW:
        attr |= BACKGROUND_RED | BACKGROUND_GREEN;
        break;
    case TC_BACKGROUND_BLUE:
        attr |= BACKGROUND_BLUE;
        break;
    case TC_BACKGROUND_MAGENTA:
        attr |= BACKGROUND_RED | BACKGROUND_BLUE;
        break;
    case TC_BACKGROUND_CYAN:
        attr |= BACKGROUND_BLUE | BACKGROUND_GREEN;
        break;
    case TC_BACKGROUND_WHITE:
        attr |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;
    }
    if (colors & TC_BACKGROUND_BRIGHT)
        attr |= BACKGROUND_INTENSITY;

    HANDLE stream_handle = 0;
    int const stream_fd = fileno(bjam_stream);
    if (stream_fd == ::fileno(stdout))
    {
        stream_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        out_flush();
    }
    else if (stream_fd == fileno(stderr))
    {
        stream_handle = GetStdHandle(STD_ERROR_HANDLE);
        err_flush();
    }

    if (stream_handle)
    {
        SetConsoleTextAttribute(stream_handle, attr);
    }
#else
    if (!isatty(fileno(bjam_out)))
        return;

    if (colors == TC_DEFAULT)
    {
        fputs("\33[0m", bjam_stream);
        return;
    }

    switch (colors & TC_MASK_FOREGROUND)
    {
        case TC_FOREGROUND_BLACK:
            fputs("\33[0;30m", bjam_stream);
            break;
        case TC_FOREGROUND_RED:
            fputs("\33[0;31m", bjam_stream);
            break;
        case TC_FOREGROUND_GREEN:
            fputs("\33[0;32m", bjam_stream);
            break;
        case TC_FOREGROUND_YELLOW:
            fputs("\33[0;33m", bjam_stream);
            break;
        case TC_FOREGROUND_BLUE:
            fputs("\33[34m", bjam_stream);
            break;
        case TC_FOREGROUND_MAGENTA:
            fputs("\33[0;35m", bjam_stream);
            break;
        case TC_FOREGROUND_CYAN:
            fputs("\33[0;36m", bjam_stream);
            break;
        case TC_FOREGROUND_WHITE:
            fputs("\33[37m", bjam_stream);
            break;
    }

    switch (colors & TC_MASK_BACKGROUND)
    {
        case TC_BACKGROUND_BLACK:
            fputs("\33[40m", bjam_stream);
            break;
        case TC_BACKGROUND_RED:
            fputs("\33[41m", bjam_stream);
            break;
        case TC_BACKGROUND_GREEN:
            fputs("\33[42m", bjam_stream);
            break;
        case TC_BACKGROUND_YELLOW:
            fputs("\33[43m", bjam_stream);
            break;
        case TC_BACKGROUND_BLUE:
            fputs("\33[44m", bjam_stream);
            break;
        case TC_BACKGROUND_MAGENTA:
            fputs("\33[45m", bjam_stream);
            break;
        case TC_BACKGROUND_CYAN:
            fputs("\33[46m", bjam_stream);
            break;
        case TC_BACKGROUND_WHITE:
            fputs("\33[47m", bjam_stream);
            break;
    }

    if ((colors & TC_FOREGROUND_BRIGHT) || (colors & TC_BACKGROUND_BRIGHT))
        fputs("\33[1m", bjam_stream);
#endif
}

void set_color_default(FILE* bjam_stream)
{
    set_color(bjam_stream, TC_DEFAULT);
}
