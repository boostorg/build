/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#include "mod_stdinfo.h"

#include "cwd.h"
#include "events.h"
#include "ext_bfgroup_lyra.h"
#include "ext_nlohmann_json.hpp"
#include "jam.h"
#include "output.h"
#include "pathsys.h"
#include "startup.h"

#include <cstdio>
#include <string>
#include <utility>

namespace b2 {

std_info & std_info::ref()
{
	static std_info info;
	return info;
}

void std_info::add_capability(
	string_view id, std::unique_ptr<std_capability> && capability)
{
	if (capabilities.count(id) == 0) return;
	capabilities[id] = std::move(capability);
}

void std_info::set_info_out_filename(const std::string & f)
{
	if (info_out_filename == f) return;
	info_out_filename = f;
	if (info_out_filename.empty())
	{
		if (info_out_ec != 0) remove_event_callback(info_out_ec);
	}
	else
	{
		info_out_ec = add_event_callback(event_tag::pre_build,
			std::function<void()>([]() { std_info::ref().do_info_out(); }));
		globs.set_debug_level(0);
	}
}

void std_info::do_info_out()
{
	if (info_out_filename.empty()) return;
	nlohmann::json out;
	out["$schema"] = "std_info-1.0.0.json";
	out["std_info"] = "1.0.0";
	for (auto & capability : capabilities)
	{
		auto name = capability.second->get_info_name();
		auto versions = capability.second->get_info_versions();
		if (versions.size() == 1)
			out[name] = versions[0];
		else
			out[name] = versions;
	}
	if (info_out_filename == "-")
		std::puts(out.dump(2).c_str());
	else
	{
		std::string filename;
		if (!b2::paths::is_rooted(info_out_filename))
			filename
				= b2::paths::normalize(b2::cwd_str() + "/" + info_out_filename);
		else
			filename = b2::paths::normalize(info_out_filename);
		FILE * file = std::fopen(filename.c_str(), "w");
		bool success = file != nullptr;
		if (success)
		{
			success = std::fputs(out.dump(2).c_str(), file) != EOF;
			std::fclose(file);
		}
		if (success)
		{
			out_printf("...wrote std-info to '%s'...\n", filename.c_str());
			b2::clean_exit(b2::exit_result::success);
		}
		else
		{
			err_printf(
				"...writing std-info to '%s' FAILED...\n", filename.c_str());
			b2::clean_exit(b2::exit_result::failure);
		}
	}
}

void std_info_module::declare_args()
{
	b2::args::lyra_cli().add_argument(lyra::opt([](bool) {
		std_info::ref().set_info_out_filename("-");
	})
			.name("--std-info")
			.help("Output capability information."));
	b2::args::lyra_cli().add_argument(lyra::opt(
		[](const std::string & s) { std_info::ref().set_info_out_filename(s); },
		"std-info-out")
			.name("--std-info-out")
			.help("Path to file to output capability information (default is console)."
				  " "
				  "Can be '-' to output to console."));
}

} // namespace b2
