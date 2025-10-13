/*
	board_esp32_timer.c - timer configuration for Espressif ESP32
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

#include "../../hard_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/timers.h"
#include "driver/timer.h"

#define TIMER_COUNT_ZERO 0U // value for setting timer tick count to 0

uint8_t timersStarted = 0U; // stores timer started state

typedef struct hw_timer_s {
	uint8_t group;
	uint8_t num;
} hard_timer_t;

static hard_timer_t timerGroups[4] = {
	{0,0}, {1,0},  {0,1},  {1,1}
};

// hardware timer pointers
hard_timer_t *timers[] = {
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

hard_timer_t *nullTimer;

/**
 * Gets timer based on desired timer
 * 
 * @param timer timer to select
 * 
 * @return pointer to timer selected
 */
hard_timer_t** getTimer(hardware_timer_t timer) {

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
void setTimerStarted(hardware_timer_t timer, bool state) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		if (state) {
			timersStarted |= (1 << timer);
		}
		else {
			timersStarted &= (~(1 << timer));
		}
	}
}

/**
 * Gets if timer was initialized
 * 
 * @param timerPtr timer to test
 * 
 * @return if timer was initialized
 */
bool timerInitializedInternal(hard_timer_t** timerPtr) {
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (*timerPtr != NULL) {
		return true;
	}

	return false;
}

bool hardTimerInitialized(hardware_timer_t timer) {
	hard_timer_t** timerPtr = getTimer(timer);
	return timerInitializedInternal(timerPtr);
}

bool hardTimerStarted(hardware_timer_t timer) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		return !!((1 << timer) & timersStarted);
	}
	return false;
}

bool IRAM_ATTR timerFunctionWrapper(void *arg) {
	void (*fn)(void) = arg;
	fn();
	return false;
}

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {

	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (!timerInitializedInternal(timerPtr)) {

		timer_config_t config = {
			.divider = scalar,
			.counter_dir = true,
			.counter_en = TIMER_PAUSE,
			.alarm_en = TIMER_ALARM_DIS,
			.auto_reload = false,
        };
		*timerPtr = &timerGroups[timer];

		timer_init((*timerPtr) -> group, (*timerPtr) -> num, &config);
		timer_set_counter_value((*timerPtr) -> group, (*timerPtr) -> num, 0);
		timer_start((*timerPtr) -> group, (*timerPtr) -> num);
		timer_isr_callback_add((*timerPtr) -> group, (*timerPtr) -> num, timerFunctionWrapper, function, 0);

		return true;
	}
	return false;
}

bool deconstructHardTimer(hardware_timer_t timer) {

	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (timerInitializedInternal(timerPtr)) {

		cancelHardTimer(timer);
		timer_isr_callback_remove((*timerPtr) -> group, (*timerPtr) -> num);
		timer_deinit((*timerPtr) -> group, (*timerPtr) -> num);
		*timerPtr = NULL;
		return true;
	}
	
	return false;
}

bool cancelHardTimer(hardware_timer_t timer) {
	
	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (hardTimerStarted(timer)) {

		timer_set_alarm((*timerPtr) -> group, (*timerPtr) -> num, false);
		timer_pause((*timerPtr) -> group, (*timerPtr) -> num);
		timer_set_counter_value((*timerPtr) -> group, (*timerPtr) -> num, TIMER_COUNT_ZERO);
		setTimerStarted(timer, false);
		return true;
	}

	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (timerInitializedInternal(timerPtr) && !hardTimerStarted(timer)) {

		timer_set_alarm_value((*timerPtr) -> group, (*timerPtr) -> num, timerTicks);
		timer_set_auto_reload((*timerPtr) -> group, (*timerPtr) -> num, true);
		timer_set_alarm((*timerPtr) -> group, (*timerPtr) -> num, true);
		timer_start((*timerPtr) -> group, (*timerPtr) -> num);
		setTimerStarted(timer, true);
		return true;
	}

	return false;
}

#endif