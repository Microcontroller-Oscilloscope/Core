/*
	hard_timer.h - timer definitions for each board
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

#ifndef HARD_TIMER_H
#define HARD_TIMER_H

#include "compile_flags/compile_flags.h"

// available timers to use
#if NUM_TIMERS > 0
	#define HARD_TIMER_ID_OSC // oscilloscope data collection enable
#endif
#if NUM_TIMERS > 1
	#define HARD_TIMER_ID_LED // status LED enable
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Stops hardware timer from executing
 * 
 * @param timer timer to stop
 * 
 * @return if timer was successfully canceled
 */
bool cancelHardTimer(hardware_timer_t timer);

/**
 * Starts hardware timer execution
 * 
 * @param timer timer to start
 * @param function function to call back
 * @param scalar scalar for function call
 * @param timerTicks timer ticks per cycle
 * 
 * @return if timer was successfully set
 */
bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks);

/**
 * Gets if selected timer was started
 * 
 * @param timer timer to check
 * 
 * @return if timer was started
 */
bool hardTimerStarted(hardware_timer_t timer);

// codes when getting hard timer stats
enum HardTimerStatusReturn {
	HARD_TIMER_OK, // hard timer stats retrieved
	HARD_TIMER_FREQ_OUT_OF_RANGE, // target frequency was unobtainable
	HARD_TIMER_SLIGHTLY_OFF, // retrieved values that aren't completely accurate
	HARD_TIMER_FAIL, // failed to get timer values
};

/**
 * Gets hard timer stats for target frequency
 * 
 * @param freq pointer to desired frequency in Hz
 * @param timer pointer to timer ID
 * @param scalar pointer to scalar value
 * @param timerTicks pointer to desired tick count
 * 
 * @return result of getting timer stats
 * 
 * @note freq value is changed to actual freq if values are slightly off
 */
enum HardTimerStatusReturn getHardTimerStats(uint32_t *freq, hardware_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks);

#ifdef __cplusplus
}
#endif

#if NUM_TIMERS > NUM_TIMERS_AVAILABLE
	#error NUM_TIMERS too large
#endif

#if NUM_TIMERS_AVAILABLE > 0 && NUM_TIMERS <= 0
	#error NUM_TIMERS too small
#endif

#endif