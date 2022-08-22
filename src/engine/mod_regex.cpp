/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#include "mod_regex.h"

#include "mem.h"
#include "regexp.h"

#include <memory>
#include <numeric>

using namespace b2;

namespace {
using regex_ptr = std::unique_ptr<regexp, std::function<void(regexp *)>>;

regex_ptr make_regex(const string_t & pattern)
{
	// TODO: Fix regcomp to not const-cast pattern.
	return regex_ptr(
		regcomp(pattern.c_str()), [](regexp * r) { BJAM_FREE(r); });
}

regex_ptr make_regex(const char * pattern)
{
	// TODO: Fix regcomp to not const-cast pattern.
	return regex_ptr(regcomp(pattern), [](regexp * r) { BJAM_FREE(r); });
}
} // namespace

list_ref b2::regex_split(
	const std::tuple<value_ref, value_ref> & string_separator)
{
	list_ref result;
	string_t string { std::get<0>(string_separator) };
	auto re = make_regex(std::get<1>(string_separator)->str());
	auto pos = string.c_str();
	auto prev = pos;
	while (regexec(re.get(), pos))
	{
		result.push_back(prev, re->startp[0]);
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

list_ref b2::regex_split_each(list_cref to_split, value_ref separator)
{
	list_ref result;
	auto re = make_regex(separator->str());
	for (auto string : to_split)
	{
		auto pos = string->str();
		auto prev = pos;
		while (regexec(re.get(), pos))
		{
			result.push_back(prev, re->startp[0]);
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

list_ref b2::regex_match(
	value_ref pattern, value_ref string, const std::vector<int_t> & indices)
{
	list_ref result;
	auto re = make_regex(pattern->str());
	if (regexec(re.get(), string->str()))
	{
		if (!indices.empty())
		{
			for (int_t index : indices)
			{
				if (index < NSUBEXP)
					result.push_back(re->startp[index], re->endp[index]);
			}
		}
		else
		{
			for (int_t index = 1;
				 index < NSUBEXP && re->startp[index] != nullptr; ++index)
				result.push_back(re->startp[index], re->endp[index]);
		}
	}
	return result;
}

list_ref b2::regex_transform(
	list_cref list, value_ref pattern, const std::vector<int_t> & indices)
{
	list_ref result;
	auto re = make_regex(pattern);
	if (!indices.empty())
	{
		for (auto string : list)
		{
			if (regexec(re.get(), string->str()))
				for (int_t index : indices)
				{
					if (index < NSUBEXP && re->startp[index] < re->endp[index])
						result.push_back(re->startp[index], re->endp[index]);
				}
		}
	}
	else
	{
		for (auto string : list)
		{
			if (regexec(re.get(), string->str()))
				if (re->startp[1] != nullptr && re->startp[1] < re->endp[1])
					result.push_back(re->startp[1], re->endp[1]);
		}
	}
	return result;
}

value_ref b2::regex_escape(
	value_ref string, value_ref symbols, value_ref escape_symbol)
{
	string_t result = string;
	string_t::size_type i = 0;
	for (i = result.find_first_of(symbols, i); i != string_t::npos;
		 i = result.find_first_of(symbols, i))
	{
		result.insert(i, escape_symbol);
		i += escape_symbol->as_string().size + 1;
	}
	return value_ref(result);
}

namespace b2 { namespace {
string_t regex_replace(
	const char * string, regex_ptr::pointer re, const char * replacement)
{
	std::string result;
	auto pos = string;
	while (regexec(re, pos))
	{
		result.append(pos, re->startp[0]);
		result.append(replacement);
		/* Handle empty matches */
		if (*pos == '\0')
			break;
		else if (pos == re->endp[0])
			pos++;
		else
			pos = re->endp[0];
	}
	result.append(pos);
	return result;
}
}} // namespace b2::

value_ref b2::regex_replace(const std::tuple<value_ref, value_ref, value_ref> &
		string_match_replacement)
{
	value_ref string = std::get<0>(string_match_replacement);
	auto re = make_regex(std::get<1>(string_match_replacement)->str());
	value_ref replacement = std::get<2>(string_match_replacement);
	return value_ref(
		regex_replace(string->str(), re.get(), replacement->str()));
}

list_ref b2::regex_replace_each(
	list_cref list, value_ref match, value_ref replacement)
{
	list_ref result;
	auto re = make_regex(match->str());
	for (auto string : list)
	{
		result.push_back(
			regex_replace(string->str(), re.get(), replacement->str()));
	}
	return result;
}

const char * b2::regex_module::init_code = R"jam(
rule __test__ ( )
{
    import assert ;

    assert.result a b c : split "a/b/c" / ;
    assert.result "" a b c : split "/a/b/c" / ;
    assert.result "" "" a b c : split "//a/b/c" / ;
    assert.result "" a "" b c : split "/a//b/c" / ;
    assert.result "" a "" b c "" : split "/a//b/c/" / ;
    assert.result "" a "" b c "" "" : split "/a//b/c//" / ;
    # assert.result "" a b c "" : split "abc" "" ;
    # assert.result "" "" : split "" "" ;
    assert.result "<x>y" "z" "<a>b" "c" "<d>e" "f" : split "<x>y\\z\\<a>b\\c\\<d>e\\f" "[\\/]" ;

    assert.result a b c "" a "" b c "" "" : split-list "a/b/c" "/a//b/c//" : / ;

    assert.result a c b d
        : match (.)(.)(.)(.) : abcd : 1 3 2 4 ;
    assert.result a b c d
        : match (.)(.)(.)(.) : abcd ;
    assert.result ababab cddc
        : match "((ab)*)([cd]+)" : abababcddc : 1 3 ;
    assert.result "a:"
        : match "(^.:)" : "a:/b" ;

    assert.result a.h c.h
        : transform <a.h> \"b.h\" <c.h> : <(.*)> ;

    assert.result a.h b.h c.h
        : transform <a.h> \"b.h\" <c.h> : "<([^>]*)>|\"([^\"]*)\"" : 1 2 ;

    assert.result "^<?xml version=\"1.0\"^>"
        : escape "<?xml version=\"1.0\">" : "&|()<>^" : "^" ;

    assert.result "<?xml version=\\\"1.0\\\">"
        : escape "<?xml version=\"1.0\">" : "\\\"" : "\\" ;

    assert.result "string&nbsp;string&nbsp;" : replace "string string " " " "&nbsp;" ;
    assert.result "&nbsp;string&nbsp;string" : replace " string string" " " "&nbsp;" ;
    assert.result "string&nbsp;&nbsp;string" : replace "string  string" " " "&nbsp;" ;
    assert.result "-" : replace "&" "&" "-" ;
    # assert.result "x" : replace "" "" "x" ;
    # assert.result "xax" : replace "a" "" "x" ;
    # assert.result "xaxbx" : replace "ab" "" "x" ;
    assert.result "a-b-c-d" : replace "a_b.c/d" "[_./]" "-" ;

    assert.result "-" "a-b" : replace-list "&" "a&b" : "&" : "-" ;
}
)jam";
