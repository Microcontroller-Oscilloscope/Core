/*
	board_pico.cpp - configuration for all Raspberry Pi Picos
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
#include "../../../timer.h"

#define NULL_REPEATING_TIMER 0

// hardware timers
struct repeating_timer timers[NUM_TIMERS];

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

storage_t initialized = 0U; // stores timer initialization state
storage_t started = 0U; // stores timer started state

/**
 * Gets timer based on desired timer
 * 
 * @param timer timer to select
 * 
 * @return pointer to timer selected
 */
struct repeating_timer* getTimer(hardware_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return &timers[timer];
	}
	return nullptr;
}

/**
 * Sets timer initialization state
 * 
 * @param timer timer to set
 * @param state whether or not timer is initialized
 */
void setTimerInitialized(hardware_timer_t timer, bool state) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		if (state) {
			initialized |= (1 << timer);
		}
		else {
			initialized &= (~(1 << timer));
		}
	}
}

/**
 * Sets timer started state
 * 
 * @param timer timer to set
 * @param state whether or not timer is started
 */
void setTimerStarted(hardware_timer_t timer, bool state) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		if (state) {
			started |= (1 << timer);
		}
		else {
			started &= (~(1 << timer));
		}
	}
}

bool timerInitialized(hardware_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return !!((1 << timer) & initialized);
	}
	return false;
}

bool timerStarted(hardware_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return !!((1 << timer) & started);
	}
	return false;
}

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {
	if (!timerInitialized(timer)) {
		setTimerInitialized(timer, true);
		return true;
	}
	return false;
}

bool deconstructHardTimer(hardware_timer_t timer) {

	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (timerInitialized(timer)) {
		cancel_repeating_timer(timerPtr);
		setTimerStarted(timer, false);
		setTimerInitialized(timer, false);
		return true;
	}

	return false;
}

bool cancelHardTimer(hardware_timer_t timer) {

	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (timerStarted(timer)) {
		cancel_repeating_timer(timerPtr);
		setTimerStarted(timer, false);
		return true;
	}

	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (!timerStarted(timer)) {
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