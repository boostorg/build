/*
Copyright 2024 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#ifndef B2_EVENTS_H
#define B2_EVENTS_H

#include "config.h"
#include "jam_fwd.h"

#include <cstdint>
#include <functional>

namespace b2 {

enum class event_tag : uint16_t
{
	unknown = 0,
	// Indicate that we are about to start the build itself. (jam.cpp)
	pre_build,
	// Signal that we are about to execute a command. (make1.cpp)
	pre_exec_cmd,
	// Build completed, or got skipped, and we are on the way out. (jam.cpp)
	exit_main
};

template <typename F>
uint64_t add_event_callback(
	event_tag tag, std::function<F> && call, int32_t priority = 0);
void remove_event_callback(uint64_t e);

void trigger_event_pre_build();
void trigger_event_pre_exec_cmd(TARGET * t);
void trigger_event_exit_main(int status);

} // namespace b2

#endif
