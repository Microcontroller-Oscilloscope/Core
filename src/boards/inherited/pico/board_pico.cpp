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

struct repeating_timer timers[NUM_TIMERS]; // hardware timers

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

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {
	return true;
}

bool deconstructHardTimer(hardware_timer_t timer) {
	return cancelHardTimer(timer);
}

bool cancelHardTimer(hardware_timer_t timer) {

	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	return cancel_repeating_timer(timerPtr);
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	struct repeating_timer* timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (scalar == SCALAR_MS) {
		return add_repeating_timer_ms(timerTicks, function, NULL, timerPtr);
	}
	else if (scalar == SCALAR_US) {
		return add_repeating_timer_us(timerTicks, function, NULL, timerPtr);
	}
	return false;
}

#endif