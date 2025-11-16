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

#endif

bool setHardTimerFunction(hard_timer_t timer, hard_timer_function_ptr_t function, void* params) {
	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	hardTimerFunctions[timer] = function;
	hardTimerParams[timer] = params;

	#ifndef NO_TIMER_CALLBACK_SUPPORT

		switch(timer) {
			#if NUM_TIMERS >= 1
				case(HARD_TIMER0):
					hardTimerCallbacks[timer] = timerCallback0;
				break;
			#endif
			#if NUM_TIMERS >= 2
				case(HARD_TIMER1):
					hardTimerCallbacks[timer] = timerCallback1;
				break;
			#endif
			#if NUM_TIMERS >= 3
				case(HARD_TIMER2):
					hardTimerCallbacks[timer] = timerCallback2;
				break;
			#endif
			#if NUM_TIMERS >= 4
				case(HARD_TIMER3):
					hardTimerCallbacks[timer] = timerCallback3;
				break;
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