/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "mod_string.h"
#include <algorithm>

static const char *whitespace_chars = " \t\n";

std::string b2::string_whitespace()
{
    return whitespace_chars;
}

std::vector<std::string> b2::string_chars(std::string s)
{
    std::vector<std::string> chars;
    chars.reserve(s.size());
    for (auto ch : s)
        chars.emplace_back(1, ch);
    return chars;
}

std::string b2::string_abbreviate(std::string s)
{
    // Anything less than 4 characters gets no abbreviation.
    if (s.size() < 4)
        return s;
    // Separate the initial letter in case it's a vowel.
    auto first_c = s[0];
    s.erase(0, 1);
    // Drop trailing "ing".
    if (s.compare(s.size() - 3, 3, "ing") == 0)
        s.erase(s.size() - 3);
    // Reduce all doubled characters to one.
    s.erase(std::unique(s.begin(), s.end()), s.end());
    // Chop all vowels out of the remainder.
    s.erase(std::remove_if(
                s.begin(), s.end(),
                [](std::string::value_type c) {
                    // Sorted by English vowel frequency.
                    return c == 'e' || c == 'E' ||
                           c == 'a' || c == 'A' ||
                           c == 'o' || c == 'O' ||
                           c == 'i' || c == 'I' ||
                           c == 'u' || c == 'U';
                }),
            s.end());
    // Shorten remaining consonants to 4 characters.
    if (s.size() > 4)
        s.erase(4);
    // Glue the initial character back on to the front.
    s.insert(0, 1, first_c);

    return s;
}

std::string b2::string_join(const std::vector<std::string> &strings, optval<std::string> separator)
{
    if (strings.empty())
        return "";
    if (!separator.has_value())
        separator = "";
    std::string result = strings.front();
    auto strings_i = strings.cbegin() + 1;
    auto strings_e = strings.cend();
    for (; strings_i != strings_e; ++strings_i)
    {
        result += separator;
        result += *strings_i;
    }
    return result;
}

std::vector<std::string> b2::string_words(std::string s, const std::vector<std::string> &whitespace)
{
    std::string delimiters{whitespace_chars};
    if (!whitespace.empty())
        delimiters = string_join(whitespace, "");
    std::vector<std::string> words;
    std::string::size_type word_begin = 0;
    std::string::size_type word_ws = s.find_first_of(delimiters);
    while (word_ws != std::string::npos)
    {
        words.push_back(s.substr(word_begin, word_ws - word_begin));
        word_begin = s.find_first_not_of(delimiters, word_ws + 1);
        word_ws = s.find_first_of(delimiters, word_begin);
    }
    if (word_begin < s.size())
        words.push_back(s.substr(word_begin));
    return words;
}

bool b2::string_is_whitespace(optval<std::string> s)
{
    return !s.has_value() || s->find_first_not_of(whitespace_chars) == std::string::npos;
}
