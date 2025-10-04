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
	 * Type Defines
	****************************/

	#include <avr/pgmspace.h>

	typedef const PROGMEM char memCharString;

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE 1024 // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 1 // amount of CPU cores available to board
	#endif

	typedef uint8_t PinMode;

	#ifndef INPUT
		#define INPUT 0x0
	#endif
	#ifndef OUTPUT
		#define OUTPUT 0x1
	#endif
	#ifndef INPUT_PULLUP
		#define INPUT_PULLUP 0x2
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

	#define NUM_TIMERS_AVAILABLE 3 // amount of hardware timers available
	#ifndef NUM_TIMERS
		#define NUM_TIMERS 3 // amount of hardware timers to use
	#endif

	// available hardware timers
	#define HARD_TIMER(id) CONCATENATE(HARD_TIMER, id) // timer #id

	typedef enum {

		#if NUM_TIMERS >= 1
			HARD_TIMER(0), // 8 bit counter
		#endif
		#if NUM_TIMERS >= 2
			HARD_TIMER(1), // 16 bit counter
		#endif
		#if NUM_TIMERS >= 3
			HARD_TIMER(2), // 8 bit counter
		#endif
	} hardware_timer_t; // hardware timer type

	// available pre scalars
	typedef enum {
		SCALAR_1, // timer prescalar of 1, timers 0-2
		SCALAR_8, // timer prescalar of 8, timers 0-2
		SCALAR_32, // timer prescalar of 32, timer 2
		SCALAR_64, // timer prescalar of 64, timers 0-2
		SCALAR_128, // timer prescalar of 128, timer 2
		SCALAR_256, // timer prescalar of 256, timers 0-2
		SCALAR_1024, // timer prescalar of 1024, timers 0-2
	} prescalar_t; // pre scalar type

	// other timer definitions
	typedef uint16_t timertick_t; // timer tick type
	typedef void (*hard_timer_function_ptr_t) (void); // timer callback function pointer

	// timer references

	/**
	 * Timer #id function reference
	 * 
	 * @param id timer id to select
	 */
	#define HARD_TIMER_REFERENCE(id) VOID_FUNCTION_NAME

	// timer functions

	#include <avr/interrupt.h>

	/**
	 * Selects timer function from given id
	 * 
	 * F_CPU = 16,000,000
	 * 
	 * F_DER = desired frequency (Hz)
	 * 
	 * timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 * F_DER = F_CPU / [scalar * (timerTicks + 1)]
	 * 
	 * @param id timer id to select
	 * 
	 * @note HARD_TIMER_FUNCTION({id}) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 8-bit counter for timer 0,2 and 16-bit for timer 1
	 * @warning scalars: 1, 8, 64, 256, 1024. timer 2 additional scalars: 32, 128
	 */
	#define HARD_TIMER_FUNCTION(id) ISR (CONCATENATE3(TIMER, id, _COMPA_vect))

	#define HARD_TIMER_END() // end of function for timers

	/****************************
	 * LED Timer Config
	****************************/

	#define HARD_TIMER_LED_INDEX 1 // hardware timer index for status LEDs
	#define HARD_TIMER_LED_SCALAR SCALAR_1024 // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER (F_CPU / 1000000L) // multiplier for timer ticks

	/****************************
	 * Test Timer Config
	****************************/

	#ifdef __TEST_CASES__

		// slow testing flags
		#define HARD_TIMER_TEST_INDEX 1 // hardware timer index for testing
		#define HARD_TIMER_TEST_SCALAR SCALAR_1024 // pre scalar for test
		#define HARD_TIMER_TEST_MULTIPLIER (F_CPU / 1000000L) // multiplier for testing timer ticks
		// override default since Uno timer equation isn't linear when multiplying tick values
		#define HARD_TIMER_TEST_COUNT_BUFFER 1 // amount timer can be off of goal

		// fast testing flags 125,000Hz
		#define HARD_TIMER_FAST_TEST_INDEX 1 // hardware timer index for testing
		#define HARD_TIMER_FAST_TEST_SCALAR SCALAR_8 // pre scalar for test
		#define HARD_TIMER_FAST_TEST_MULTIPLIER 1 // multiplier for testing timer ticks
		#define HARD_TIMER_FAST_TEST_COUNT_TARGET 125000 // target count for timer
		#define HARD_TIMER_FAST_TEST_DELAY 15 // target timer ticks for data collected
		#define HARD_TIMER_FAST_TEST_COUNT_BUFFER 1 // amount timer can be off of goal

	#endif

#endif
#endif