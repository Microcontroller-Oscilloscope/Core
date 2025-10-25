/*
	board_pico_timer.c - timer configuration for all Raspberry Pi Picos
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

#include "../../board.h"

#ifdef PICO

#include <pico/time.h>
#include "../../../hard_timer.h"

#define THOUSAND 1000

// hardware timers
struct repeating_timer timers[NUM_TIMERS];
struct repeating_timer nullTimer;

#if NUM_TIMERS <= 8
	typedef uint8_t storage_t; // storage type for timer states
#elif NUM_TIMERS <= 16
	typedef uint16_t storage_t; // storage type for timer states
#elif NUM_TIMERS <= 32
	typedef uint32_t storage_t; // storage type for timer states
#elif NUM_TIMERS <= 64
	typedef uint64_t storage_t; // storage type for timer states
#else
	#error TOO MANY TIMERS! Reduce NUM_TIMERS to 64 or less
#endif

storage_t timersStarted = 0U; // stores timer started state

/**
 * Gets timer based on desired timer
 * 
 * @param timer timer to select
 * 
 * @return pointer to timer selected
 */
struct repeating_timer* getTimer(hard_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return &timers[timer];
	}
	return &nullTimer;
}

/**
 * Sets timer started state
 * 
 * @param timer timer to set
 * @param state whether or not timer is started
 */
void setTimerStarted(hard_timer_t timer, bool state) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		if (state) {
			timersStarted |= (1 << timer);
		}
		else {
			timersStarted &= (~(1 << timer));
		}
	}
}

hard_timer_t getNextTimer(void) {
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!hardTimerStarted(i)) {
			return (hard_timer_t)i;
		}
	}
	return HARD_TIMER_INVALID;
}

enum HardTimerStatusReturn getHardTimerStats(freq_t *freq, hard_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks) {
	if (*freq > FREQ_MAX) {
		return HARD_TIMER_FREQ_OUT_OF_RANGE;
	}

	enum HardTimerStatusReturn status = HARD_TIMER_OK;

	// freq doesn't divide evenly with us
	if (FREQ_MAX % *freq != 0) {
		status = HARD_TIMER_SLIGHTLY_OFF;
	}

	freq_t target = FREQ_MAX / *freq;

	if (target % THOUSAND == 0 && status == HARD_TIMER_OK) {
		*scalar = SCALAR_MS;
		*timerTicks = target / THOUSAND;
	}
	else {
		*scalar = SCALAR_US;
		*timerTicks = target;
	}

	if (*scalar == SCALAR_MS) {
		*freq = FREQ_MAX / *timerTicks * THOUSAND;
	}
	else if (*scalar == SCALAR_US) {
		*freq = FREQ_MAX / *timerTicks;
	}

	*timer = getNextTimer();
	
	return status;
}

bool hardTimerStarted(hard_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return !!((1 << timer) & timersStarted);
	}
	return false;
}

bool cancelHardTimer(hard_timer_t timer) {

	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (hardTimerStarted(timer)) {
		cancel_repeating_timer(timerPtr);
		setTimerStarted(timer, false);
		return true;
	}

	return false;
}

bool setHardTimer(hard_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (!hardTimerStarted(timer)) {
		if (scalar == SCALAR_MS) {
			if (add_repeating_timer_ms(-timerTicks, function, NULL, timerPtr)) {
				setTimerStarted(timer, true);
				return true;
			}
		}
		else if (scalar == SCALAR_US) {
			if (add_repeating_timer_us(-timerTicks, function, NULL, timerPtr)) {
				setTimerStarted(timer, true);
				return true;
			}
		}
	}

	return false;
}

#endif