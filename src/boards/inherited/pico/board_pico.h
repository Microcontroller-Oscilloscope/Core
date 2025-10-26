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
	 * 
	 * 2 are used for system timing on each core
	 * 
	 * Only 14 timers after api usage
	****************************/

	#define FREQ_MAX 1000000 // max frequency user set timer can be

	#ifndef NUM_TIMERS
		#define NUM_TIMERS 14 // amount of hardware timers to use
	#endif

	typedef bool hard_timer_return_t; // return type of timer function
	typedef struct repeating_timer* hard_timer_param_t; // parameter type of timer function

	/**
	 * Sets function to run in RAM if possible
	 * 
	 * @param function function name
	 * 
	 * @note {return_type} RUN_IN_RAM({function_name}) {function_name} ({params}) {{content}}
	 */
	#define RUN_IN_RAM(function) __not_in_flash(__STRING( function ))

	/**
	 * Returns from timer function
	 * 
	 * @note hard_timer_return_t RUN_IN_RAM({function_name}) {function_name}(hard_timer_param_t emptyParams) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning emptyParams doesn't include any user input parameters
	 * @warning timerTicks: time in ms or us
	 * @warning scalar: SCALAR_MS (millis) or SCALAR_US (micros)
	 */
	#define HARD_TIMER_END() return true

	/****************************
	 * Test Timer Config
	****************************/

	#define TEST_FAST_FREQ 300000 // target frequency

#endif
#endif