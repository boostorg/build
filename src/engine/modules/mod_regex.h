/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef B2_MOD_REGEX_H
#define B2_MOD_REGEX_H

#include "../config.h"
#include "../bind.h"
#include "../optval.h"
#include "../types.h"
#include <string>
#include <tuple>
#include <vector>

namespace b2
{
std::vector<string_t> regex_split(
    const std::tuple<string_t, string_t> &string_separator);
std::vector<string_t> regex_split_each(
    const std::vector<string_t> &to_split, const string_t &separator);
std::vector<string_t> regex_match(
    const string_t &pattern, const string_t &string,
    const std::vector<int_t> &indices);
std::vector<string_t> regex_transform(
    const std::vector<string_t> &list,
    const string_t &pattern,
    const std::vector<int_t> &indices);
string_t regex_escape(
    const string_t &string,
    const string_t &symbols,
    const string_t &escape_symbol);
string_t regex_replace(
    const std::tuple<string_t, string_t, string_t> &string_match_replacement);
std::vector<string_t> regex_replace_each(
    const std::vector<string_t> list,
    const string_t &match,
    const string_t &replacement);

struct regex_module
    : b2::bind::module_<regex_module>
{
    const char *module_name = "regex";

    template <typename Binder>
    void def(Binder &binder)
    {
        binder
            .def(&regex_split, "split", "string"*_1 + "separator"*_1)
            .def(&regex_split_each, "split-list", "list"*_n | "separator"*_1)
            .def(&regex_match, "match", "pattern"*_1 | "string"*_1 | "indices"*_n)
            .def(&regex_transform, "transform", "list"*_n | "pattern"*_1 | "indices"*_n)
            .def(&regex_escape, "escape", "string"*_1 | "symbols"*_1 | "escape-symbol"*_1)
            .def(&regex_replace, "replace", "string"*_1 + "match"*_1 + "replacement"*_1)
            .def(&regex_replace_each, "replace-list", "list"*_n | "match"*_1 | "replacement"*_1);
    }
};
} // namespace b2

#endif
