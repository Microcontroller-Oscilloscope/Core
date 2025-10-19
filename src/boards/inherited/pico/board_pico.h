/*
	board_pico.h - configuration flags for all Raspberry Pi Picos
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

#ifndef BOARD_PICO_H
#define BOARD_PICO_H

#include "../../board_generic.h"

#ifdef PICO

	/****************************
	 * Type Defines
	****************************/

	#include <avr/pgmspace.h>

	typedef const char memCharString;

	#define PROG_FLASH PROGMEM // storage specifier for flash space

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE FLASH_NVM_SIZE // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 2 // amount of CPU cores available to board
	#endif

	#ifndef SERIAL_PRINTF
		#define SERIAL_PRINTF // uses printf as serial
	#endif

	#ifndef NVM_INTERNAL
		#define NVM_INTERNAL // uses internal nvm functions
	#endif

	/****************************
	 * NVM Config
	****************************/

	#ifndef __NVM_BEGIN__
		#define __NVM_BEGIN__ // Calls begin function for NVM
	#endif
	#ifndef __NVM_BEGIN_SIZE__
		#define __NVM_BEGIN_SIZE__ // Calls begin function with NVM size
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 16 hardware alarm timers available
	****************************/

	#include <pico/time.h>

	#define NUM_TIMERS_AVAILABLE 16 // amount of hardware timers available
	#ifndef NUM_TIMERS
		#define NUM_TIMERS 16 // amount of hardware timers to use
	#endif

	// available hardware timers
	#define HARD_TIMER(id) CONCATENATE(HARD_TIMER, id) // timer #id

	typedef enum { // hardware timer type
		#if NUM_TIMERS >= 1
			HARD_TIMER(0), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 2
			HARD_TIMER(1), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 3
			HARD_TIMER(2), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 4
			HARD_TIMER(3), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 5
			HARD_TIMER(4), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 6
			HARD_TIMER(5), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 7
			HARD_TIMER(6), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 8
			HARD_TIMER(7), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 9
			HARD_TIMER(8), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 10
			HARD_TIMER(9), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 11
			HARD_TIMER(10), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 12
			HARD_TIMER(11), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 13
			HARD_TIMER(12), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 14
			HARD_TIMER(13), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 15
			HARD_TIMER(14), // 64 bit counter
		#endif
		#if NUM_TIMERS >= 16
			HARD_TIMER(15), // 64 bit counter
		#endif
	} hardware_timer_t; // hardware timer type

	// available pre scalars
	typedef enum {
		SCALAR_MS, // timer prescalar for milli seconds
		SCALAR_US, // timer prescalar micro seconds
	} prescalar_t; // pre scalar type

	// other timer definitions
	typedef int64_t timertick_t; // timer tick type
	typedef bool (*hard_timer_function_ptr_t) (struct repeating_timer*); // timer callback function pointer

	// timer references

	/**
	 * Timer #id function reference
	 * 
	 * @param id timer id to select
	 */
	#define HARD_TIMER_REFERENCE(id) CONCATENATE(hardTimerFunction, id)

	// timer functions

	/**
	 * Starter function for timer #id
	 * 
	 * @param id numeric value to append to function
	 * 
	 * @note HARD_TIMER_FUNCTION({id}) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning timerTicks: time in ms or us
	 * @warning scalar: SCALAR_MS (millis) or SCALAR_US (micros)
	 */
	#define HARD_TIMER_FUNCTION(id) bool CONCATENATE(hardTimerFunction, id)(struct repeating_timer *t)

	#define HARD_TIMER_END() return true // end of function for timers

	#define HARD_TIMER_TICK_MULTIPLIER 1 // multiplier for all timers

	/****************************
	 * LED Timer Config
	****************************/

	#define HARD_TIMER_LED_INDEX 0 // hardware timer index for status LEDs
	#define HARD_TIMER_LED_SCALAR SCALAR_MS // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER HARD_TIMER_TICK_MULTIPLIER // multiplier for led timer ticks

	/****************************
	 * Test Timer Config
	****************************/

	//#ifdef __TEST_CASES__

		// slow testing flags
		#define HARD_TIMER_TEST_INDEX 0 // hardware timer index for testing
		#define HARD_TIMER_TEST_SCALAR SCALAR_MS // pre scalar for test
		#define HARD_TIMER_TEST_MULTIPLIER HARD_TIMER_TICK_MULTIPLIER // multiplier for testing timer ticks

		// fast testing flags 200,000Hz
		#define HARD_TIMER_FAST_TEST_INDEX 0 // hardware timer index for testing
		#define HARD_TIMER_FAST_TEST_SCALAR SCALAR_US // pre scalar for test
		#define HARD_TIMER_FAST_TEST_MULTIPLIER HARD_TIMER_TICK_MULTIPLIER // multiplier for testing timer ticks
		#define HARD_TIMER_FAST_TEST_COUNT_TARGET 200000 // target count for timer
		#define HARD_TIMER_FAST_TEST_DELAY 5 // target timer ticks for data collected
		#define HARD_TIMER_FAST_TEST_COUNT_BUFFER 1 // amount timer can be off of goal

	//#endif

#endif
#endif