/*
	common_timer.c - timer definitions for each board
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

#include "common_timer.h"

// functions to execute at end of ISR
hard_timer_function_ptr_t hardTimerFunctions[NUM_TIMERS];
// function parameters to pass
void* hardTimerParams[NUM_TIMERS];

#ifndef NO_TIMER_CALLBACK_SUPPORT

	// callback functions for linking to ISR
	callback_ptr_t hardTimerCallbacks[NUM_TIMERS];

	#if SUPPORTED_ESP32

		typedef bool callback_ret_t;
		#define CALLBACK_RETURN() return false

		#if ESP_IDF_VERSION_MAJOR == 4
			#define CALL_PARAMS void *params
		#elif ESP_IDF_VERSION_MAJOR == 5
			#define CALL_PARAMS gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *params
		#endif

	#elif SUPPORTED_PICO

		typedef bool callback_ret_t;
		#define CALLBACK_RETURN() return true
		#define CALL_PARAMS repeating_timer_t *rt

	#endif

	/**
	 * Creates callback functions for each timer
	 * 
	 * @param num timer number to set
	 */
	#define TIMER_CALLBACK_PROTOTYPE(num) \
		static callback_ret_t CONCATENATE(timerCallback, num)(CALL_PARAMS) { \
			((void(*)())hardTimerFunctions[num])(hardTimerParams[num]); \
			CALLBACK_RETURN(); \
		}

	#if NUM_TIMERS >= 1
		TIMER_CALLBACK_PROTOTYPE(0)
	#endif
	#if NUM_TIMERS >= 2
		TIMER_CALLBACK_PROTOTYPE(1)
	#endif
	#if NUM_TIMERS >= 3
		TIMER_CALLBACK_PROTOTYPE(2)
	#endif
	#if NUM_TIMERS >= 4
		TIMER_CALLBACK_PROTOTYPE(3)
	#endif
	#if NUM_TIMERS >= 5
		TIMER_CALLBACK_PROTOTYPE(4)
	#endif
	#if NUM_TIMERS >= 6
		TIMER_CALLBACK_PROTOTYPE(5)
	#endif
	#if NUM_TIMERS >= 7
		TIMER_CALLBACK_PROTOTYPE(6)
	#endif
	#if NUM_TIMERS >= 8
		TIMER_CALLBACK_PROTOTYPE(7)
	#endif
	#if NUM_TIMERS >= 9
		TIMER_CALLBACK_PROTOTYPE(8)
	#endif
	#if NUM_TIMERS >= 10
		TIMER_CALLBACK_PROTOTYPE(9)
	#endif
	#if NUM_TIMERS >= 11
		TIMER_CALLBACK_PROTOTYPE(10)
	#endif
	#if NUM_TIMERS >= 12
		TIMER_CALLBACK_PROTOTYPE(11)
	#endif
	#if NUM_TIMERS >= 13
		TIMER_CALLBACK_PROTOTYPE(12)
	#endif
	#if NUM_TIMERS >= 14
		TIMER_CALLBACK_PROTOTYPE(13)
	#endif
	#if NUM_TIMERS >= 15
		TIMER_CALLBACK_PROTOTYPE(14)
	#endif
	#if NUM_TIMERS >= 16
		TIMER_CALLBACK_PROTOTYPE(15)
	#endif

#endif

/**
 * Simplifies callback test cases
 * 
 * @param num timer number
 */
#define CALLBACK_TEST_CASE(num) \
	case(CONCATENATE(HARD_TIMER, num)): \
		hardTimerCallbacks[num] = CONCATENATE(timerCallback, num); \
	break;

bool setHardTimerFunction(hard_timer_t timer, hard_timer_function_ptr_t function, void* params) {
	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	hardTimerFunctions[timer] = function;
	hardTimerParams[timer] = params;

	#ifndef NO_TIMER_CALLBACK_SUPPORT

		switch(timer) {
			#if NUM_TIMERS >= 1
				CALLBACK_TEST_CASE(0)
			#endif
			#if NUM_TIMERS >= 2
				CALLBACK_TEST_CASE(1)
			#endif
			#if NUM_TIMERS >= 3
				CALLBACK_TEST_CASE(2)
			#endif
			#if NUM_TIMERS >= 4
				CALLBACK_TEST_CASE(3)
			#endif
			#if NUM_TIMERS >= 5
				CALLBACK_TEST_CASE(4)
			#endif
			#if NUM_TIMERS >= 6
				CALLBACK_TEST_CASE(5)
			#endif
			#if NUM_TIMERS >= 7
				CALLBACK_TEST_CASE(6)
			#endif
			#if NUM_TIMERS >= 8
				CALLBACK_TEST_CASE(7)
			#endif
			#if NUM_TIMERS >= 9
				CALLBACK_TEST_CASE(8)
			#endif
			#if NUM_TIMERS >= 10
				CALLBACK_TEST_CASE(9)
			#endif
			#if NUM_TIMERS >= 11
				CALLBACK_TEST_CASE(10)
			#endif
			#if NUM_TIMERS >= 12
				CALLBACK_TEST_CASE(11)
			#endif
			#if NUM_TIMERS >= 13
				CALLBACK_TEST_CASE(12)
			#endif
			#if NUM_TIMERS >= 14
				CALLBACK_TEST_CASE(13)
			#endif
			#if NUM_TIMERS >= 15
				CALLBACK_TEST_CASE(14)
			#endif
			#if NUM_TIMERS >= 16
				CALLBACK_TEST_CASE(15)
			#endif
			default:
			break;
		}

	#endif

	return true;
}

callback_ptr_t getHardTimerCallback(hard_timer_t timer) {
	#ifndef NO_TIMER_CALLBACK_SUPPORT
		if (timer == HARD_TIMER_INVALID) {
			return NULL;
		}
		return hardTimerCallbacks[timer];
	#else
		return NULL;
	#endif
}