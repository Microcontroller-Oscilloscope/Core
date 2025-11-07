/*
	default_timer.c - timer configuration for Arduino
	Copyright (C) 2025 Camren Chraplak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../compile_flags/compile_flags.h"

#if !SUPPORTED_PLATFORM

/**
 * Since each platform has their own ways they create and manage timers
 * and there is no Arduino timer interface, the default methods won't work
 */

#include "../osc_common/common_timer.h"

hard_timer_t claimTimer(struct hardTimerPriority *priority) {
	return HARD_TIMER_INVALID;
}

bool unclaimTimer(hard_timer_t timer) {
	return false;
}

bool hardTimerClaimed(hard_timer_t timer) {
	return false;
}

bool hardTimerStarted(hard_timer_t timer) {
	return false;
}

bool cancelHardTimer(hard_timer_t timer) {
	return false;
}

bool setHardTimer(hard_timer_t *timer, freq_t *freq, hard_timer_function_ptr_t function, timer_priority_t priority) {
	return false;
}

#endif