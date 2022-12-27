/*
Copyright 2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#include "tasks.h"

b2::task_group::task_group() {}

void b2::task_group::queue(const std::function<void()> & f) { f(); }

void b2::task_group::wait() {}

b2::task_executor::task_executor(unsigned parallelism /* = 0 */) {}

std::unique_ptr<b2::task_group> b2::task_executor::make(
	unsigned parallelism /* = 0 */)
{
	return std::make_unique<b2::task_group>();
}

b2::task_executor & b2::task_executor::get()
{
	task_executor e;
	return e;
}
