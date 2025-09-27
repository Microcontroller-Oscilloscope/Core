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

#include <Esp.h>

hw_timer_t *timer0 = NULL; // timer 0 pointer
hw_timer_t *timer1 = NULL; // timer 1 pointer
hw_timer_t *timer2 = NULL; // timer 2 pointer
hw_timer_t *timer3 = NULL; // timer 3 pointer

/**
 * Initializes hardware timer
 * 
 * @param timer hardware timer to configure
 * @param scalar scalar to count to
 */
void initHardTimerInternal(hw_timer_t **timer, void (*function)(void), prescalar_t scalar, hardware_timer_t timerID) {
	*timer = timerBegin(timerID, scalar, true);
	timerAttachInterrupt(*timer, function, true);
}

void initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {
	if (timer == HARD_TIMER0) {
		initHardTimerInternal(&timer0, function, scalar, HARD_TIMER0);
	}
	else if (timer == HARD_TIMER1) {
		initHardTimerInternal(&timer1, function, scalar, HARD_TIMER1);
	}
	else if (timer == HARD_TIMER2) {
		initHardTimerInternal(&timer2, function, scalar, HARD_TIMER2);
	}
	else if (timer == HARD_TIMER3) {
		initHardTimerInternal(&timer3, function, scalar, HARD_TIMER3);
	}
}

/**
 * Cancels hardware timer
 * 
 * @param timer hardware timer to cancel
 */
void cancelHardTimerInternal(hw_timer_t *timer) {
	timerAlarmDisable(timer);
	timerStop(timer);
}

void cancelHardTimer(hardware_timer_t timer) {
	if (timer == HARD_TIMER0) {
		cancelHardTimerInternal(timer0);
	}
	else if (timer == HARD_TIMER1) {
		cancelHardTimerInternal(timer1);
	}
	else if (timer == HARD_TIMER2) {
		cancelHardTimerInternal(timer2);
	}
	else if (timer == HARD_TIMER3) {
		cancelHardTimerInternal(timer3);
	}
}

/**
 * Sets hardware timer duration
 * 
 * @param timer hardware timer to start
 * @param timerTicks timer ticks for function triggering
 */
void setHardTimerInternal(hw_timer_t *timer, timertick_t timerTicks) {
	timerAlarmWrite(timer, timerTicks, true);
	timerAlarmEnable(timer);
	timerStart(timer);
}

void setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	if (timer == HARD_TIMER0) {
		setHardTimerInternal(timer0, timerTicks);
	}
	else if (timer == HARD_TIMER1) {
		setHardTimerInternal(timer1, timerTicks);
	}
	else if (timer == HARD_TIMER2) {
		setHardTimerInternal(timer2, timerTicks);
	}
	else if (timer == HARD_TIMER3) {
		setHardTimerInternal(timer3, timerTicks);
	}
}

#endif