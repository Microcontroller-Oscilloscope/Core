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

typedef enum {
	SCALAR_MS, // timer prescalar for milli seconds
	SCALAR_US, // timer prescalar micro seconds
} prescalar_t; // pre scalar type
typedef int64_t timertick_t; // timer tick type

// hardware timers
struct repeating_timer timers[NUM_TIMERS];

#if NUM_TIMERS <= 8
	typedef uint8_t storage_t; // storage type for timer states
#elif NUM_TIMERS <= 16
	typedef uint16_t storage_t; // storage type for timer states
#endif

storage_t timersStarted = 0U; // stores timer started state
storage_t timersClaimed = 0U; // stores timer claimed state

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
	return NULL;
}

/**
 * Sets timer started state
 * 
 * @param timer timer to set
 * @param state whether or not timer is started
 */
void setTimerStarted(hard_timer_t timer, bool state) {

	if (timer == HARD_TIMER_INVALID) {
		return;
	}
	if (state) {
		timersStarted |= (((storage_t)1) << timer);
	}
	else {
		timersStarted &= (~(((storage_t)1) << timer));
	}
}

/**
 * Sets timer claimed state
 * 
 * @param timer timer to set
 * @param state whether or not timer is claimed
 */
void setTimerClaimed(hard_timer_t timer, bool state) {

	if (timer == HARD_TIMER_INVALID) {
		return;
	}
	if (state) {
		timersClaimed |= (((storage_t)1) << (timer));
	}
	else {
		timersClaimed &= (~(((storage_t)1) << (timer)));
	}
}

/**
 * Gets next unstarted and unclaimed timer
 * 
 * @return available timer
 */
hard_timer_t getNextTimer(void) {
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!hardTimerStarted(i) && !hardTimerClaimed(i)) {
			return (hard_timer_t)i;
		}
	}
	return HARD_TIMER_INVALID;
}

hard_timer_t claimTimer(struct hardTimerPriority *priority) {

	hard_timer_t timer = getNextTimer();
	if (timer != HARD_TIMER_INVALID) {
		setTimerClaimed(timer, true);
	}
	return timer;
}

bool unclaimTimer(hard_timer_t timer) {
	if (hardTimerClaimed(timer)) {
		setTimerClaimed(timer, false);
		return true;
	}
	return false;
}

bool hardTimerClaimed(hard_timer_t timer) {

	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	return !!(timersClaimed & (((storage_t)1) << (timer)));
}

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
enum HardTimerStatusReturn getHardTimerStats(freq_t *freq, hard_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks) {

	enum HardTimerStatusReturn status = HARD_TIMER_OK;

	// freq doesn't divide evenly with us
	if (FREQ_MAX % *freq != 0) {
		status = HARD_TIMER_SLIGHTLY_OFF;
	}

	//target in us
	freq_t targetUS = FREQ_MAX / *freq;

	if (targetUS % THOUSAND == 0 && status == HARD_TIMER_OK) {
		*scalar = SCALAR_MS;
		*timerTicks = targetUS / THOUSAND;
	}
	else {
		*scalar = SCALAR_US;
		*timerTicks = targetUS;
	}

	if (*scalar == SCALAR_MS) {
		*freq = FREQ_MAX / (*timerTicks * THOUSAND);
	}
	else if (*scalar == SCALAR_US) {
		*freq = FREQ_MAX / *timerTicks;
	}

	if ((!hardTimerClaimed(*timer) && hardTimerStarted(*timer)) || *timer == HARD_TIMER_INVALID) {
		*timer = getNextTimer();
	}
	
	if (*timer == HARD_TIMER_INVALID) {
		return HARD_TIMER_FAIL;
	}

	return status;
}

bool hardTimerStarted(hard_timer_t timer) {

	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	return !!((((storage_t)1) << timer) & timersStarted);
}

bool cancelHardTimer(hard_timer_t timer) {

	if (hardTimerStarted(timer)) {
		struct repeating_timer* timerPtr = getTimer(timer);
		if (!cancel_repeating_timer(timerPtr)) {
			return false;
		}
		setTimerStarted(timer, false);
		return true;
	}

	return false;
}

bool setHardTimer(hard_timer_t *timer, freq_t *freq, hard_timer_function_ptr_t function, timer_priority_t priority) {

	if (function == NULL || freq == NULL || timer == NULL) {
		return false;
	}
	if (*freq == (freq_t)0 || *freq > FREQ_MAX) {
		return false;
	}

	prescalar_t scalar;
	timertick_t timerTicks;
	
	if (getHardTimerStats(freq, timer, &scalar, &timerTicks) == HARD_TIMER_FAIL) {
		return false;
	}

	if (!hardTimerStarted(*timer)) {
		struct repeating_timer* timerPtr = getTimer(*timer);
		if (scalar == SCALAR_MS) {
			if (add_repeating_timer_ms(-timerTicks, function, NULL, timerPtr)) {
				setTimerStarted(*timer, true);
				return true;
			}
		}
		else if (scalar == SCALAR_US) {
			if (add_repeating_timer_us(-timerTicks, function, NULL, timerPtr)) {
				setTimerStarted(*timer, true);
				return true;
			}
		}
	}

	return false;
}

#endif