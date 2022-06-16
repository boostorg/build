/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_REGEX_H
#define B2_MOD_REGEX_H

#include "../config.h"

#include "../bind.h"
#include "../lists.h"
#include "../optval.h"
#include "../types.h"
#include "../value.h"

#include <tuple>
#include <vector>

namespace b2 {
list_ref regex_split(const std::tuple<value_ref, value_ref> & string_separator);
list_ref regex_split_each(list_cref to_split, value_ref separator);
list_ref regex_match(
	value_ref pattern, value_ref string, const std::vector<int_t> & indices);
list_ref regex_transform(
	list_cref list, value_ref pattern, const std::vector<int_t> & indices);
value_ref regex_escape(
	value_ref string, value_ref symbols, value_ref escape_symbol);
value_ref regex_replace(const std::tuple<value_ref, value_ref, value_ref> &
		string_match_replacement);
list_ref regex_replace_each(
	list_cref list, value_ref match, value_ref replacement);

struct regex_module : b2::bind::module_<regex_module>
{
	const char * module_name = "regex";

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
	}
};
} // namespace b2

#endif
