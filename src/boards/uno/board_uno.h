/*
	board_uno.h - configuration flags for Arduino Uno
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

#ifndef BOARD_UNO_H
#define BOARD_UNO_H

#include "../board_generic.h"

#ifdef ARDUINO_AVR_UNO
	#define UNOR3
	#define BOARD_FOUND
#endif

#ifdef UNOR3

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE 1024 // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 1 // amount of CPU cores available to board
	#endif

	#ifndef STATUS_LED_PIN
		#define STATUS_LED_PIN 13 // pin for status LED
	#endif

	/****************************
	 * NVM Config
	****************************/

	#if !NVM_CHECK()
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 3 hardware timers available
	****************************/

	#include <stdint.h>

	enum HARDWARE_TIMER {
		HARD_TIMER0, // hardware timer 0, 8 bit counter
		HARD_TIMER1, // hardware timer 1, 16 bit counter
		HARD_TIMER2, // hardware timer 2, 8 bit counter
	};

	#define HARD_TIMER_LED HARD_TIMER1 // hardware timer for status LEDs

	enum PRE_SCALAR {
		SCALAR_1, // timer prescalar of 1, timers 0-2
		SCALAR_8, // timer prescalar of 8, timers 0-2
		SCALAR_32, // timer prescalar of 32, timer 2
		SCALAR_64, // timer prescalar of 64, timers 0-2
		SCALAR_128, // timer prescalar of 128, timer 2
		SCALAR_256, // timer prescalar of 256, timers 0-2
		SCALAR_1024, // timer prescalar of 1024, timers 0-2
	};

	/**
	 * Stops hardware timer from executing
	 * 
	 * @param timer timer to stop
	 */
	void cancelHardTimer(HARDWARE_TIMER timer);

	/**
	 * Configure hardware timer
	 * 
	 * @param timer timer to start
	 * @param scalar scalar of timer
	 * @param timerTicks compare ticks
	 * 
	 * @note F_CPU = 16,000,000
	 * @note F_DER = desired frequency (Hz)
	 * @note timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 */
	void setHardTimer(HARDWARE_TIMER timer, PRE_SCALAR scalar, uint16_t timerTicks);

#endif
#endif