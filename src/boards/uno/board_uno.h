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

	#include "board_uno_pins.h"

	#ifndef NUM_IO_PINS
		#define NUM_IO_PINS 20 // number pins available to controller
	#endif

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
		#define NVM_SIZE 1024 // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 1 // amount of CPU cores available to board
	#endif

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
		#define STATUS_LED_PIN INTERNAL_LED // pin for status LED
	#endif

	#ifndef SERIAL_INTERNAL
		#define SERIAL_INTERNAL // uses boards internal functions for transmission
	#endif

	#ifndef SERIAL_INTERNAL
		#define SERIAL_PRINTF // uses printf as serial
	#endif

	#ifdef SERIAL_INTERNAL
		#ifndef NO_FLOAT_PRINT_SUPPORT
			#define NO_FLOAT_PRINT_SUPPORT
		#endif
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

	

	/****************************
	 * Timer Config
	 * 
	 * Only 3 hardware timers available
	****************************/

	#define FREQ_MAX 1000000 // max frequency user set timer can be

	#ifndef NUM_TIMERS
		#define NUM_TIMERS 3 // amount of hardware timers to use
	#endif

	typedef enum {
		SCALAR_1, // timer prescalar of 1, timers 0-2
		SCALAR_8, // timer prescalar of 8, timers 0-2
		SCALAR_32, // timer prescalar of 32, timer 2
		SCALAR_64, // timer prescalar of 64, timers 0-2
		SCALAR_128, // timer prescalar of 128, timer 2
		SCALAR_256, // timer prescalar of 256, timers 0-2
		SCALAR_1024, // timer prescalar of 1024, timers 0-2
	} prescalar_t; // pre scalar type
	typedef uint16_t timertick_t; // timer tick type

	typedef void hard_timer_return_t; // return type of timer function
	typedef void* hard_timer_param_t; // parameter type of timer function

	/**
	 * Returns from timer function
	 * 
	 * F_CPU = 16,000,000
	 * 
	 * F_DER = desired frequency (Hz)
	 * 
	 * timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 * F_DER = F_CPU / [scalar * (timerTicks + 1)]
	 * 
	 * @note hard_timer_return_t RUN_IN_RAM({function_name}) {function_name}(hard_timer_param_t emptyParams) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning emptyParams doesn't include any user input parameters
	 * @warning 8-bit counter for timer 0,2 and 16-bit for timer 1
	 * @warning scalars: 1, 8, 64, 256, 1024. timer 2 additional scalars: 32, 128
	 */
	#define HARD_TIMER_END()

	/****************************
	 * Test Timer Config
	****************************/

	#define TEST_FAST_FREQ 120000 // target frequency

#endif
#endif