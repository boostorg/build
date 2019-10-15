/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "mod_regex.h"
#include <regex>

using namespace b2;

namespace
{
template <typename Pattern>
std::regex make_regex(Pattern &&pattern)
{
    return std::regex{std::forward<Pattern>(pattern), std::regex::extended};
}
} // namespace

std::vector<string_t> b2::regex_split(
    const std::tuple<string_t, string_t> &string_separator)
{
    std::vector<string_t> result;
    string_t string{std::get<0>(string_separator)};
    std::regex separator = make_regex(std::get<1>(string_separator));
    std::sregex_token_iterator tokens_i{string.cbegin(), string.cend(), separator, {-1, 0}};
    std::sregex_token_iterator tokens_e{};
    std::sregex_token_iterator tokens_z{};
    while (tokens_i != tokens_e)
    {
        result.push_back(*tokens_i++);
        tokens_z = tokens_i;
        if (tokens_i != tokens_e)
            tokens_i++;
    }
    if (tokens_z != tokens_e)
    {
        result.emplace_back("");
    }
    return result;
}

std::vector<string_t> b2::regex_split_each(
    const std::vector<string_t> &to_split, const string_t &separator)
{
    std::vector<string_t> result;
    std::regex separator_re = make_regex(separator);
    std::sregex_token_iterator tokens_e{};
    for (const string_t &string : to_split)
    {
        std::sregex_token_iterator tokens_i{
            string.cbegin(), string.cend(), separator_re, {-1, 0}};
        std::sregex_token_iterator tokens_z{};
        while (tokens_i != tokens_e)
        {
            result.push_back(*tokens_i++);
            tokens_z = tokens_i;
            if (tokens_i != tokens_e)
                tokens_i++;
        }
        if (tokens_z != tokens_e)
        {
            result.emplace_back("");
        }
    }
    return result;
}

std::vector<string_t> b2::regex_match(
    const string_t &pattern, const string_t &string,
    const std::vector<int_t> &indices)
{
    std::regex pattern_re = make_regex(pattern);
    if (indices.empty())
    {
        std::smatch match;
        if (std::regex_search(string, match, pattern_re))
            return {++match.cbegin(), match.cend()};
        return {};
    }
    else
        return {
            std::sregex_token_iterator{
                string.cbegin(), string.cend(), pattern_re, indices},
            std::sregex_token_iterator{}};
}

std::vector<string_t> b2::regex_transform(
    const std::vector<string_t> &list,
    const string_t &pattern,
    const std::vector<int_t> &indices)
{
    std::vector<string_t> result;
    std::regex pattern_re = make_regex(pattern);
    std::sregex_token_iterator tokens_e{};
    if (indices.empty())
    {
        for (const string_t &string : list)
        {
            std::sregex_token_iterator tokens_i{
                string.cbegin(), string.cend(), pattern_re, 1};
            if (tokens_i != tokens_e && tokens_i->length() > 0)
                result.push_back(*tokens_i);
        }
    }
    else
    {
        for (const string_t &string : list)
        {
            std::sregex_token_iterator tokens_i{
                string.cbegin(), string.cend(), pattern_re, indices};
            for (; tokens_i != tokens_e; ++tokens_i)
            {
                if (tokens_i->length() > 0)
                    result.push_back(*tokens_i);
            }
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
