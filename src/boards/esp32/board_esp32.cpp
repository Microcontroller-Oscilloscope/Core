/*
	board_esp32.cpp - configuration for Espressif ESP32
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

#include "../board.h"

#ifdef ESP32DEVC

#include <esp32-hal-timer.h>

#define TIMER_COUNT_ZERO 0U // value for setting timer tick count to 0

// hardware timer pointers
hw_timer_t *timers[NUM_TIMERS] = {
	#if NUM_TIMERS >= 1
		NULL,
	#endif
	#if NUM_TIMERS >= 2
		NULL,
	#endif
	#if NUM_TIMERS >= 3
		NULL,
	#endif
	#if NUM_TIMERS >= 4
		NULL,
	#endif
};

/**
 * Gets timer based on desired timer
 * 
 * @param timer timer to select
 * 
 * @return pointer to timer selected
 */
hw_timer_t** getTimer(hardware_timer_t timer) {

	if (timer >= 0 && timer < NUM_TIMERS) {
		return &timers[timer];
	}
	return nullptr;
}

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {

	hw_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (*timerPtr == NULL) {
		*timerPtr = timerBegin(timer, scalar, true);
		timerAttachInterrupt(*timerPtr, function, true);
		return true;
	}

	return false;
}

bool deconstructHardTimer(hardware_timer_t timer) {

	hw_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (*timerPtr != NULL) {

		if (cancelHardTimer(timer)) {
			timerDetachInterrupt(*timerPtr);
			timerEnd(*timerPtr);
			*timerPtr = NULL;
			return true;
		}

	}
	
	return false;
}

bool cancelHardTimer(hardware_timer_t timer) {
	
	hw_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (*timerPtr != NULL) {
		timerAlarmDisable(*timerPtr);
		timerStop(*timerPtr);
		timerWrite(*timerPtr, TIMER_COUNT_ZERO);
		return true;
	}

	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	hw_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == nullptr) {
		return false;
	}

	if (*timerPtr != NULL) {
		if (!timerAlarmEnabled(*timerPtr)) {
			timerAlarmWrite(*timerPtr, timerTicks, true);
			timerAlarmEnable(*timerPtr);
			timerStart(*timerPtr);
			return true;
		}
	}

	return false;
}

#endif