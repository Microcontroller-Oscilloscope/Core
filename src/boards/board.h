/*
	board.h - configuration flags for Core
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

#ifndef BOARD_H
#define BOARD_H

#include "board_generic.h"
#include "uno/board_uno.h"
#include "esp32/board_esp32.h"
#include "pico/board_pico.h"

#define DEFAULT_BAUD_RATE 9600 // default baud rate for microcontrollers

/****************************
 * Checks Required Flags
****************************/

#ifndef BOARD_FOUND
	#error Board not found. Not officially supported.
#endif

#ifndef BAUD_RATE
	#define BAUD_RATE DEFAULT_BAUD_RATE // baud rate to print to console
	#error Define 'BAUD_RATE' in 'platformio.ini' file with '-D BAUD_RATE=${this.monitor_speed}' under 'build_flags =' under '[env]'
#endif

#ifndef NVM_SIZE
	#define NVM_SIZE 0 // size in bytes of NVM
	#error NVM_SIZE must be whole number 1 or greater
#endif

#ifndef CORE_COUNT
	#define CORE_COUNT 1 // amount of CPU cores available to board
	#error CORE_COUNT set to 1
#else
	#if CORE_COUNT < 1
		#error CORE_COUNT must be whole number 1 or greater
	#elif CORE_COUNT > 2
		#error CORE_COUNT must be whole number 2 or less
	#endif
#endif

/****************************
 * Changes Flags When Testing
****************************/

#ifndef __TEST_CASES__
	#if defined(PIO_UNIT_TESTING) || defined(UNIT_TEST)
		#define __TEST_CASES__ // indicator if compiling for testing
	#endif
#endif

// removes debug flags that print to console
#ifdef __TEST_CASES__
	#undef __ERROR_DEBUG__
	#undef __NVM_DEBUG__
#endif

/****************************
 * Checks Includes
 * TODO: Add checks
****************************/

/*#ifndef __ENABLED_ENV__
	#error __ENABLED_ENV__ not found. Include 'ini/generic.ini' under 'extra_configs = '
#endif

#ifndef __TEST_CASES__
	#ifndef __ENABLED_RELEASE__
		#error __ENABLED_RELEASE__ not found
	#endif
	#ifndef __ENABLED_BOARD_RELEASE__
		#error __ENABLED_BOARD_RELEASE__ not found
	#endif
#else
	#ifndef __ENABLED_TEST__
		#error __ENABLED_TEST__ not found
	#endif
	#ifndef __ENABLED_BOARD_TEST__
		#error __ENABLED_BOARD_TEST__ not found
	#endif
#endif*/

#endif