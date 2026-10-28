/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_MOD_STDINFO_H
#define B2_MOD_STDINFO_H

#include "config.h"

#include "bind.h"
#include "lists.h"
#include "mod_args.h"
#include "optval.h"
#include "strview.h"
#include "value.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace b2 {

class std_capability : public object
{
	public:
	virtual std::string get_info_name() const = 0;
	virtual std::vector<std::string> get_info_versions() const = 0;
};

class std_info : public object
{
	public:
	static std_info & ref();
	void add_capability(
		string_view id, std::unique_ptr<std_capability> && capability);
	void set_info_out_filename(const std::string & f);
	void do_info_out();

	private:
	std::unordered_map<std::string, std::unique_ptr<std_capability>> capabilities;
	std::string info_out_filename;
	uint64_t info_out_ec = 0;
};

struct std_info_module
	: b2::bind::module_<std_info_module>
	, b2::args::declaration_<std_info_module>
{
	const char * module_name = "stdinfo";

	template <class Binder>
	void def(Binder & binder)
	{
		binder.loaded();
	}

	static void declare_args();
};

} // namespace b2

#endif
