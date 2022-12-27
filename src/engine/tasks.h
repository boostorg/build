/*
Copyright 2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_TASKS_H
#define B2_TASKS_H

#include "config.h"

#include "mod_sysinfo.h"

#include <functional>
#include <memory>

namespace b2 {

class task_group
{
	public:
	task_group();
	void queue(const std::function<void()> & f);
	void wait();
};

class task_executor
{
	public:
	task_executor(unsigned parallelism = 0);
	std::unique_ptr<task_group> make(unsigned parallelism = 0);
	static task_executor & get();
};

} // namespace b2

#endif
