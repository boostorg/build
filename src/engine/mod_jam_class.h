/*
Copyright 2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_CLASS_H
#define B2_MOD_CLASS_H

#include "config.h"

#include "bind.h"
#include "lists.h"
#include "value.h"

#include <tuple>

/* tag::reference[]

[[b2.reference.modules.class]]
= `class` module.

end::reference[] */

namespace b2 { namespace jam { namespace klass {

std::string make(std::tuple<value_ref, list_ref> name_arg1,
	const lists & rest,
	bind::context_ref_ context_ref);

list_ref bases(std::string class_name);

bool is_derived(value_ref class_name, list_cref class_bases);

bool is_instance(std::string value);

bool is_a(std::string instance, value_ref type);

struct class_module : b2::bind::module_<class_module>
{
	const char * module_name = "class";
	static const char * init_code;

	template <class Binder>
	void def(Binder & binder)
	{
		binder.def(&make, "new", ("class" * _1 + "args" * _n) | "rest" * _r)
			.def(&bases, "bases", "class" * _1)
			.def(&is_derived, "is-derived", "class" * _1 | "bases" * _1n)
			.def(&is_instance, "is-instance", "value" * _1)
			.def(&is_a, "is-a", "instance" * _1 | "type" * _1);
		binder.eval(init_code);
		binder.loaded();
	}
};

}}} // namespace b2::jam::klass

#endif
