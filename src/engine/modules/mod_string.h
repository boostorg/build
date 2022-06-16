/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_STRING_H
#define B2_MOD_STRING_H

#include "../config.h"

#include "../bind.h"
#include "../lists.h"
#include "../optval.h"
#include "../value.h"

#include <string>
#include <vector>

/* tag::string[]

= `string` module.

end::string[] */

namespace b2 {
/* tag::string[]

== `string_whitespace`

Returns the canonical set of whitespace characters, as a single string.

Jam::
`rule whitespace ( )`

end::string[] */
value_ref string_whitespace();

/* tag::string[]

== `string_chars`

Splits the given `string` into a list of strings composed of each character of
the `string` in sequence.

Jam::
`rule chars ( string )`

end::string[] */
list_ref string_chars(value_ref s);

/* tag::string[]

== `string_abbreviate`

Apply a set of standard transformations to string to produce an abbreviation
no more than 5 characters long.

Jam::
`rule abbreviate ( string )`

end::string[] */
value_ref string_abbreviate(value_ref s);

/* tag::string[]

== `string_join`

Concatenates the given `strings`, inserting the given `separator` between each
string.

Jam::
`rule join ( strings * : separator ? )`

end::string[] */
value_ref string_join(list_cref strings, value_ref separator);

/* tag::string[]

== `string_words`

Split a string into whitespace separated words.

Jam::
`rule words ( string : whitespace * )`

end::string[] */
list_ref string_words(std::string s, list_cref whitespace);

/* tag::string[]

== `string_is_whitespace`

Check that the given `string` is composed entirely of whitespace.

Jam::
`rule is-whitespace ( string ? )`

end::string[] */
bool string_is_whitespace(value_ref s);

struct string_module : b2::bind::module_<string_module>
{
	const char * module_name = "string";

	template <class Binder>
	void def(Binder & binder)
	{
		binder.def(&b2::string_whitespace, "whitespace")
			.def(&b2::string_chars, "chars", "string" * _1)
			.def(&b2::string_abbreviate, "abbreviate", "string" * _1)
			.def(&b2::string_join, "join", "strings" * _n | "separator" * _01)
			.def(&b2::string_words, "words", "string" * _1 | "whitespace" * _n)
			.def(&b2::string_is_whitespace, "is-whitespace", "string" * _01);
	}
};
} // namespace b2

#endif
