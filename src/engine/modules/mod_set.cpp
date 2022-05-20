/*  Copyright 2019 Rene Rivera
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "mod_set.h"
#include <algorithm>
#include <unordered_map>

void b2::set::add(const std::vector<std::string> &values)
{
    elements.insert(values.cbegin(), values.cend());
}

void b2::set::add(const set &value)
{
    elements.insert(value.elements.cbegin(), value.elements.cend());
}

bool b2::set::contains(const std::string &value) const
{
    return elements.find(value) != elements.end();
}

std::vector<std::string> b2::set::to_vector() const
{
    return {elements.cbegin(), elements.cend()};
}

std::vector<std::string> b2::set::difference(
    const std::vector<std::string> &a,
    const std::vector<std::string> &b)
{
    // Somewhat less efficient variation on difference to maintain the order
    // of sequence "a".
    std::unordered_set<std::string> rhs{b.cbegin(), b.cend()};
    std::vector<std::string> result;
    for (const std::string & val: a)
    {
        if (rhs.count(val) == 0)
            result.push_back(val);
    }
    return result;
}

std::vector<std::string> b2::set::intersection(
    const std::vector<std::string> &a,
    const std::vector<std::string> &b)
{
    std::unordered_set<std::string> lhs{a.cbegin(), a.cend()};
    std::vector<std::string> result;
    for (const std::string & val: b)
    {
        if (lhs.count(val) > 0)
            result.push_back(val);
    }
    return result;
}

bool b2::set::equal(
    const std::vector<std::string> &a,
    const std::vector<std::string> &b)
{
    std::unordered_map<std::string, unsigned> val_count;
    for (const std::string & val: a)
    {
        val_count[val] += 1;
    }
    for (const std::string & val: b)
    {
        val_count[val] += 1;
    }
    for (auto &vc: val_count)
    {
        if (vc.second == 1)
            return false;
    }
    return true;
}
