/*
	board_esp32.h - configuration flags for Espressif ESP32
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

#ifndef BOARD_ESP32_H
#define BOARD_ESP32_H

#include "../board_generic.h"

#ifdef ESP32
	#define ESP32DEVC
	#define BOARD_FOUND
#endif

#ifdef ESP32DEVC

	#if defined(CONFIG_ESPTOOLPY_MONITOR_BAUD) && !defined(PLATFORMIO)
		#ifdef BAUD_RATE
			#undef BAUD_RATE
		#endif
		#define BAUD_RATE CONFIG_ESPTOOLPY_MONITOR_BAUD
	#endif

	#include "board_esp32_pins.h"

	#ifndef NUM_IO_PINS
		#define NUM_IO_PINS 32 // number pins available to controller
	#endif

	#include <esp_attr.h>

	#ifdef PLATFORMIO
		#include <esp32-hal-gpio.h>
	#endif

	/****************************
	 * Type Defines
	****************************/

	typedef const char memCharString;

	#ifdef PLATFORMIO
		#include <pgmspace.h>
		#define PROG_FLASH PROGMEM // storage specifier for flash space
	#endif

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE FLASH_NVM_SIZE // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 2 // amount of CPU cores available to board
	#endif

	#include <hal/gpio_types.h>

	#ifndef EXTERNAL_LED_PIN
		#define EXTERNAL_STATUS_LED_PIN 23 // pin for external status LED
	#endif

	#ifndef SERIAL_PRINTF
		#define SERIAL_PRINTF // uses printf as serial
	#endif

	#ifndef IO_INTERNAL
		#define IO_INTERNAL // uses internal IO functions to set pins
	#endif

	#ifndef DELAY_INTERNAL
		#define DELAY_INTERNAL // uses internal delay functions
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
	#ifndef __NVM_BEGIN_RETURN__
		#define __NVM_BEGIN_RETURN__ // Checks return parameter of nvm begin
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 4 hardware timers available
	****************************/

	#define NUM_TIMERS_AVAILABLE 4 // amount of hardware timers available
	#ifndef NUM_TIMERS
		#define NUM_TIMERS 4 // amount of hardware timers to use
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
	} hardware_timer_t; // hardware timer type

	// available pre scalars
	typedef uint16_t prescalar_t; // pre scalar type

	// other timer definitions
	typedef uint64_t timertick_t; // timer tick type
	typedef void (*hard_timer_function_ptr_t) (void); // timer callback function pointer

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
	 * APB_CLK = 80,000,000Hz
	 * 
	 * freq = desired frequency (Hz)
	 * 
	 * freq = APB_CLK / (scalar * timerTicks)
	 * 
	 * @param id numeric value to append to function
	 * 
	 * @note HARD_TIMER_FUNCTION({id}) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 64-bit counter
	 * @warning 16-bit scalar
	 */
	#define HARD_TIMER_FUNCTION(id) void IRAM_ATTR CONCATENATE(hardTimerFunction, id) ()

	#define HARD_TIMER_END() // end of function for timers

	/****************************
	 * LED Timer Config
	****************************/
	
	#define HARD_TIMER_LED_INDEX 0 // hardware timer index for status LEDs
	#define HARD_TIMER_LED_SCALAR 80 // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER 1000 // multiplier for timer ticks

	/****************************
	 * Test Timer Config
	****************************/

	//#ifdef __TEST_CASES__

		// slow testing flags
		#define HARD_TIMER_TEST_INDEX 0 // hardware timer index for testing
		#define HARD_TIMER_TEST_SCALAR 80 // pre scalar for test
		#define HARD_TIMER_TEST_MULTIPLIER 1000 // multiplier for testing timer ticks

		// fast testing flags 200,000Hz
		#define HARD_TIMER_FAST_TEST_INDEX 0 // hardware timer index for testing
		#define HARD_TIMER_FAST_TEST_SCALAR 80 // pre scalar for test
		#define HARD_TIMER_FAST_TEST_MULTIPLIER 1 // multiplier for testing timer ticks
		#define HARD_TIMER_FAST_TEST_COUNT_TARGET 200000 // target count for timer
		#define HARD_TIMER_FAST_TEST_DELAY 5 // target timer ticks for data collected
		#define HARD_TIMER_FAST_TEST_COUNT_BUFFER 1 // amount timer can be off of goal

	//#endif

#endif
#endif