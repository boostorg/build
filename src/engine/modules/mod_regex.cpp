/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "mod_regex.h"
#include "../mem.h"
#include "../regexp.h"
#include <numeric>
#include <memory>

using namespace b2;

namespace
{
using regex_ptr = std::unique_ptr<regexp, std::function<void(regexp *)>>;

template <typename Pattern>
regex_ptr make_regex(Pattern &&pattern)
{
    return regex_ptr(
        regcomp(pattern.c_str()),
        [](regexp *r) { BJAM_FREE(r); });
}
} // namespace

std::vector<string_t> b2::regex_split(
    const std::tuple<string_t, string_t> &string_separator)
{
    std::vector<string_t> result;
    string_t string{std::get<0>(string_separator)};
    auto re = make_regex(std::get<1>(string_separator));
    auto pos = string.c_str();
    auto prev = pos;
    while (regexec(re.get(), pos))
    {
        result.emplace_back(prev, re->startp[0]);
        prev = re->endp[0];
        /* Handle empty matches */
        if (*pos == '\0')
            break;
        else if (pos == re->endp[0])
            pos++;
        else
            pos = re->endp[0];
    }
    result.push_back(pos);
    return result;
}

std::vector<string_t> b2::regex_split_each(
    const std::vector<string_t> &to_split, const string_t &separator)
{
    std::vector<string_t> result;
    auto re = make_regex(separator);
    for (const string_t &string : to_split)
    {
        auto pos = string.c_str();
        auto prev = pos;
        while (regexec(re.get(), pos))
        {
            result.emplace_back(prev, re->startp[0]);
            prev = re->endp[0];
            /* Handle empty matches */
            if (*pos == '\0')
                break;
            else if (pos == re->endp[0])
                pos++;
            else
                pos = re->endp[0];
        }
        result.push_back(pos);
    }
    return result;
}

std::vector<string_t> b2::regex_match(
    const string_t &pattern, const string_t &string,
    const std::vector<int_t> &indices)
{
    std::vector<string_t> result;
    auto re = make_regex(pattern);
    if (regexec(re.get(), string.c_str()))
    {
        if (!indices.empty())
        {
            for (int_t index : indices)
            {
                if (index < NSUBEXP)
                    result.emplace_back(re->startp[index], re->endp[index]);
            }
        }
        else
        {
            for (int_t index = 1; index < NSUBEXP && re->startp[index] != nullptr; ++index)
                result.emplace_back(re->startp[index], re->endp[index]);
        }
    }
    return result;
}

std::vector<string_t> b2::regex_transform(
    const std::vector<string_t> &list,
    const string_t &pattern,
    const std::vector<int_t> &indices)
{
    std::vector<string_t> result;
    auto re = make_regex(pattern);
    if (!indices.empty())
    {
        for (const string_t &string : list)
        {
            if (regexec(re.get(), string.c_str()))
                for (int_t index : indices)
                {
                    if (index < NSUBEXP && re->startp[index] < re->endp[index])
                        result.emplace_back(re->startp[index], re->endp[index]);
                }
        }
    }
    else
    {
        for (const string_t &string : list)
        {
            if (regexec(re.get(), string.c_str()))
                if (re->startp[1] != nullptr && re->startp[1] < re->endp[1])
                    result.emplace_back(re->startp[1], re->endp[1]);
        }
    }
    return result;
}

string_t b2::regex_escape(
    const string_t &string,
    const string_t &symbols,
    const string_t &escape_symbol)
{
    string_t result = string;
    string_t::size_type i = 0;
    for (
        i = result.find_first_of(symbols, i);
        i != string_t::npos;
        i = result.find_first_of(symbols, i))
    {
        result.insert(i, escape_symbol);
        i += escape_symbol.length() + 1;
    }
    return result;
}

string_t b2::regex_replace(
    const std::tuple<string_t, string_t, string_t> &string_match_replacement)
{
    string_t string = std::get<0>(string_match_replacement);
    const string_t &match = std::get<1>(string_match_replacement);
    const string_t &replacement = std::get<2>(string_match_replacement);
    string_t::size_type i = 0;
    for (i = string.find(match, i); i != string_t::npos; i = string.find(match, i))
    {
        string.replace(i, match.length(), replacement);
        i += replacement.length();
    }
    return string;
}

std::vector<string_t> b2::regex_replace_each(
    const std::vector<string_t> list,
    const string_t &match,
    const string_t &replacement)
{
    std::vector<string_t> result;
    for (string_t string : list)
    {
        string_t::size_type i = 0;
        for (i = string.find(match, i); i != string_t::npos; i = string.find(match, i))
        {
            string.replace(i, match.length(), replacement);
            i += replacement.length();
        }
        result.push_back(string);
    }
    return result;
}
