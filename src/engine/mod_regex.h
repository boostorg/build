/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_REGEX_H
#define B2_MOD_REGEX_H

#include "config.h"

#include "bind.h"
#include "lists.h"
#include "optval.h"
#include "types.h"
#include "value.h"

#include <tuple>
#include <vector>

/* tag::reference[]

= `regex` module.

end::reference[] */

namespace b2 {

/* tag::reference[]

== `b2::regex_split`

Returns a list of the following substrings:

. from beginning till the first occurrence of 'separator' or till the end,
. between each occurrence of 'separator' and the next occurrence,
. from the last occurrence of 'separator' till the end.

If no separator is present, the result will contain only one element.

Jam::
`rule split ( string separator )`
{CPP}::
`b2::list_ref rb2::egex_split(const std::tuple<b2::value_ref, b2::value_ref> &
string_separator);`

end::reference[] */
list_ref regex_split(const std::tuple<value_ref, value_ref> & string_separator);

/* tag::reference[]

== `b2::regex_split_each`

Returns the concatenated results of Applying regex.split to every element of
the list using the separator pattern.

Jam::
`rule split-list ( list * : separator )`
{CPP}::
`b2::list_ref b2::regex_split_each(b2::list_cref to_split, b2::value_ref
separator);`

end::reference[] */
list_ref regex_split_each(list_cref to_split, value_ref separator);

/* tag::reference[]

== `b2::regex_match`

Match string against pattern, and return the elements indicated by indices.

Jam::
`rule match ( pattern : string : indices * )`
{CPP}::
`b2::list_ref regex_match(b2::value_ref pattern, b2::value_ref string, const
std::vector<int_t> & indices);`

end::reference[] */
list_ref regex_match(
	value_ref pattern, value_ref string, const std::vector<int_t> & indices);

/* tag::reference[]

== `b2::regex_transform`

Matches all elements of `list` against the `pattern` and returns a list of
elements indicated by indices of all successful matches. If `indices` is
omitted returns a list of first parenthesized groups of all successful
matches.

Jam::
`rule transform ( list * : pattern : indices * )`
{CPP}::
`b2::list_ref regex_transform(b2::list_cref list, b2::value_ref pattern, const
std::vector<int_t> & indices);`

end::reference[] */
list_ref regex_transform(
	list_cref list, value_ref pattern, const std::vector<int_t> & indices);

/* tag::reference[]

== `b2::regex_escape`

Escapes all of the characters in symbols using the escape symbol escape-symbol
for the given string, and returns the escaped string.

Jam::
`rule escape ( string : symbols : escape-symbol )`
{CPP}::
`b2::value_ref regex_escape(b2::value_ref string,b2:: value_ref symbols,
b2::value_ref escape_symbol);`

end::reference[] */
value_ref regex_escape(
	value_ref string, value_ref symbols, value_ref escape_symbol);

/* tag::reference[]

== `b2::regex_replace`

Replaces occurrences of a match string in a given string and returns the new
string. The match string can be a regex expression.

* `string` -- The string to modify.
* `match` -- The characters to replace.
* `replacement` -- The string to replace with.

Jam::
`rule replace ( string match replacement )`
{CPP}::
`b2::value_ref regex_replace(const std::tuple<b2::value_ref, b2::value_ref,
b2::value_ref> & string_match_replacement);`

end::reference[] */
value_ref regex_replace(const std::tuple<value_ref, value_ref, value_ref> &
		string_match_replacement);

/* tag::reference[]

== `b2::regex_replace_each`

Replaces occurrences of a match string in a given list of strings and returns
a list of new strings. The match string can be a regex expression.

* `list` -- The list of strings to modify.
* `match` -- The search expression.
* `replacement` -- The string to replace with.

Jam::
`rule replace-list ( list * : match : replacement )`
{CPP}::
`b2::list_ref regex_replace_each(b2::list_cref list, b2::value_ref match,
b2::value_ref replacement);`

end::reference[] */
list_ref regex_replace_each(
	list_cref list, value_ref match, value_ref replacement);

struct regex_module : b2::bind::module_<regex_module>
{
	const char * module_name = "regex";
	static const char * init_code;

	template <typename Binder>
	void def(Binder & binder)
	{
		binder.def(&regex_split, "split", "string" * _1 + "separator" * _1)
			.def(
				&regex_split_each, "split-list", "list" * _n | "separator" * _1)
			.def(&regex_match, "match",
				"pattern" * _1 | "string" * _1 | "indices" * _n)
			.def(&regex_transform, "transform",
				"list" * _n | "pattern" * _1 | "indices" * _n)
			.def(&regex_escape, "escape",
				"string" * _1 | "symbols" * _1 | "escape-symbol" * _1)
			.def(&regex_replace, "replace",
				"string" * _1 + "match" * _1 + "replacement" * _1)
			.def(&regex_replace_each, "replace-list",
				"list" * _n | "match" * _1 | "replacement" * _1);
		binder.eval(init_code);
		binder.loaded();
	}
};
} // namespace b2

#endif
