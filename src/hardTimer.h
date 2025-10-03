/*
	hardTimer.h - timer definitions for each board
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

/**
 * All timer paths
 * 
 * base: configuration when timer hasn't been touched since startup
 * init: initHardTimer function
 * deconstruct: deconstructHardTimer function
 * cancel: cancelHardTimer function
 * set: setHardTimer function
 * 
 * Valid paths:
 * 
 * base/deconstruct -> init
 * init/cancel -> deconstruct
 * init/cancel -> set
 * set -> cancel
 * set -> deconstruct
 * 
 * Invalid paths:
 * 
 * base/deconstruct -> deconstruct
 * base/deconstruct -> cancel
 * base/deconstruct -> set
 * 
 * init/cancel -> init
 * init/cancel -> cancel
 * 
 * set -> set
 * set -> init
 */

/**
 * All timer paths
 * 
 * (x, y): x(init), y(start)
 * 
 * (0,0): deconstruct, base
 * (1,0): init, set
 * (1,1): set
 * (0,1): NOT POSSIBLE
 * 
 * Valid paths:
 * (0,0) <-> (1,0) <-> (1,1)
 * (0,0) <- (1,1)
 * 		OR
 * (0,0) -> (1,0)
 * (1,0) -> (0,0)
 * (1,0) -> (1,1)
 * (1,1) -> (0,0)
 * (1,1) -> (1,0)
 * 
 * Invalid paths:
 * 
 * (0,0) -> (0,0)
 * (0,0) -> (1,0)
 * 
 * (1,0) -> (1,0)
 * 
 * (1,1) -> (1,1)
 */

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Initializes hardware timer
 * 
 * @param timer timer to initialize
 * @param function function to call back
 * @param scalar scalar for function call
 * 
 * @return if timer was successfully initialized
 */
bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar);

/**
 * De-initializes hardware timer
 * 
 * @param timer timer to initialize
 * 
 * @return if timer was successfully de-initialized
 */
bool deconstructHardTimer(hardware_timer_t timer);

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
 * Gets if selected timer was initialized
 * 
 * @param timer timer to check
 * 
 * @return if timer was initialized
 */
bool hardTimerInitialized(hardware_timer_t timer);

/**
 * Gets if selected timer was started
 * 
 * @param timer timer to check
 * 
 * @return if timer was started
 */
bool hardTimerStarted(hardware_timer_t timer);

#if NUM_TIMERS > NUM_TIMERS_AVAILABLE
	#error NUM_TIMERS too large
#endif

#if NUM_TIMERS_AVAILABLE > 0 && NUM_TIMERS <= 0
	#error NUM_TIMERS too small
#endif

#ifdef __cplusplus
}
#endif

#endif