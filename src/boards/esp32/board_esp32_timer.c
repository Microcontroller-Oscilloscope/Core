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

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <freertos/timers.h>
#include <driver/timer.h>

#define TIMER_COUNT_ZERO 0U // value for setting timer tick count to 0
#define SCALAR_MAX UINT16_MAX // max value for timer scalar

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
 * Gets if timer was started
 * 
 * @param timerPtr timer to test
 * 
 * @return if timer was started
 */
bool timerStartedInternal(hard_timer_t** timerPtr) {
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (*timerPtr != NULL) {
		return true;
	}

	return false;
}

/**
 * function wrapper for callback function
 * 
 * @param args arguments to pass
 * 
 * @return false, don't yeild at end
 */
bool IRAM_ATTR timerFunctionWrapper(void *arg) {
	void (*fn)(void) = arg;
	fn();
	return false;
}

hardware_timer_t getNextTimer(void) {
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!hardTimerStarted(i)) {
			return (hardware_timer_t)i;
		}
	}
	return HARD_TIMER_INVALID;
}

enum HardTimerStatusReturn getHardTimerStats(freq_t *freq, hardware_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks) {
	if (*freq > FREQ_MAX) {
		return HARD_TIMER_FREQ_OUT_OF_RANGE;
	}

	enum HardTimerStatusReturn status = HARD_TIMER_OK;

	// freq doesn't divide evenly into APB_CLK
	if (APB_CLK_FREQ % *freq != 0) {
		status = HARD_TIMER_SLIGHTLY_OFF;
	}

	// scalar * timerTicks = APB_CLK / freq
	freq_t target = APB_CLK_FREQ / *freq;

	if (target <= SCALAR_MAX) {
		// scalar within max value
		*scalar = (prescalar_t)target;
		*timerTicks = 1;
	}
	else {
		// scalar not within max value
		*scalar = 1;
		*timerTicks = (timertick_t)target;

		while (*timerTicks % 2 == 0 && *scalar * 2 <= SCALAR_MAX) {
			*timerTicks /= 2;
			*scalar *= 2;
		}
	}

	*freq = APB_CLK_FREQ / (*scalar * *timerTicks);
	*timer = getNextTimer();
	return status;
}

bool hardTimerStarted(hardware_timer_t timer) {
	hard_timer_t** timerPtr = getTimer(timer);
	return timerStartedInternal(timerPtr);
}

bool cancelHardTimer(hardware_timer_t timer) {
	
	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (hardTimerStarted(timer)) {

		// cancels timer
		timer_set_alarm((*timerPtr) -> group, (*timerPtr) -> num, false);
		timer_pause((*timerPtr) -> group, (*timerPtr) -> num);
		timer_set_counter_value((*timerPtr) -> group, (*timerPtr) -> num, TIMER_COUNT_ZERO);

		// deconstructs timer
		timer_isr_callback_remove((*timerPtr) -> group, (*timerPtr) -> num);
		timer_deinit((*timerPtr) -> group, (*timerPtr) -> num);
		*timerPtr = NULL;

		return true;
	}

	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	
	hard_timer_t** timerPtr = getTimer(timer);
	if (timerPtr == &nullTimer) {
		return false;
	}

	if (!timerStartedInternal(timerPtr)) {

		// init timer
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

		// run timer
		timer_set_alarm_value((*timerPtr) -> group, (*timerPtr) -> num, timerTicks);
		timer_set_auto_reload((*timerPtr) -> group, (*timerPtr) -> num, true);
		timer_set_alarm((*timerPtr) -> group, (*timerPtr) -> num, true);
		timer_start((*timerPtr) -> group, (*timerPtr) -> num);
		return true;
	}

	return false;
}

#endif