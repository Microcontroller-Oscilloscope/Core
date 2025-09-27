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

struct repeating_timer timer0; // timer 0
struct repeating_timer timer1; // timer 1
struct repeating_timer timer2; // timer 2
struct repeating_timer timer3; // timer 3
struct repeating_timer timer4; // timer 4
struct repeating_timer timer5; // timer 5
struct repeating_timer timer6; // timer 6
struct repeating_timer timer7; // timer 7
struct repeating_timer timer8; // timer 8
struct repeating_timer timer9; // timer 9
struct repeating_timer timer10; // timer 10
struct repeating_timer timer11; // timer 11
struct repeating_timer timer12; // timer 12
struct repeating_timer timer13; // timer 13
struct repeating_timer timer14; // timer 14
struct repeating_timer timer15; // timer 15

/**
 * Gets timer based on desired timer
 * 
 * @param timer timer to select
 * 
 * @return pointer to timer selected
 */
struct repeating_timer* getTimer(hardware_timer_t timer) {
	switch (timer) {
		case(HARD_TIMER0):
			return &timer0;
		break;
		case(HARD_TIMER1):
			return &timer1;
		break;
		case(HARD_TIMER2):
			return &timer2;
		break;
		case(HARD_TIMER3):
			return &timer3;
		break;
		case(HARD_TIMER4):
			return &timer4;
		break;
		case(HARD_TIMER5):
			return &timer5;
		break;
		case(HARD_TIMER6):
			return &timer6;
		break;
		case(HARD_TIMER7):
			return &timer7;
		break;
		case(HARD_TIMER8):
			return &timer8;
		break;
		case(HARD_TIMER9):
			return &timer9;
		break;
		case(HARD_TIMER10):
			return &timer10;
		break;
		case(HARD_TIMER11):
			return &timer11;
		break;
		case(HARD_TIMER12):
			return &timer12;
		break;
		case(HARD_TIMER13):
			return &timer13;
		break;
		case(HARD_TIMER14):
			return &timer14;
		break;
		case(HARD_TIMER15):
			return &timer15;
		break;
		default:
			return nullptr;
		break;
	}
}

void initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {

}

void cancelHardTimer(hardware_timer_t timer) {
	//struct repeating_timer* timerPointer = getTimer(timer);
	cancel_repeating_timer(getTimer(timer));
}

void setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	//struct repeating_timer* timerPointer = getTimer(timer);
	if (scalar == SCALAR_MS) {
		add_repeating_timer_ms(timerTicks, function, NULL, getTimer(timer));
	}
	else if (scalar == SCALAR_US) {
		add_repeating_timer_us(timerTicks, function, NULL, getTimer(timer));
	}
}

#endif