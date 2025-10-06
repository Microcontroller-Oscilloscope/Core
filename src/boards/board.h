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
#include "pico1/board_pico1.h"
#include "pico1w/board_pico1w.h"

#ifndef DEFAULT_BAUD_RATE
	#define DEFAULT_BAUD_RATE 9600 // default baud rate for microcontrollers
#endif

#ifndef PROG_FLASH
	#define PROG_FLASH
	#define EMPTY_PROG_FLASH
#endif

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

// removes debug flags that print to console
#ifdef __TEST_CASES__
	#undef __ERROR_DEBUG__
	#undef __NVM_DEBUG__
#endif

/****************************
 * Checks Includes
****************************/

// checks flags for all configurations
#ifndef __ENABLED_ENV__
	#error '__ENABLED_ENV__' not found. Include 'ini/generic.ini' under 'extra_configs ='
#endif
#ifndef __ENABLED_BASE__
	#error '__ENABLED_BASE__' not found. Include your boards '.ini' file
#endif

#ifndef __TEST_CASES__
	// checks flags for release configurations
	#ifndef __ENABLED_RELEASE__
		#error '__ENABLED_RELEASE__' not found. Include 'ini/generic.ini' and your boards '.ini' under 'extra_configs ='. Ensure 'extends = env, base, release_env, dev_base, dev_board' is included
	#endif
#else
	// checks flags for test configurations
	#ifndef __ENABLED_TEST__
		#error '__ENABLED_TEST__' not found. Include 'ini/generic.ini' and your boards '.ini' under 'extra_configs ='. Ensure 'extends = env, base, test_env, dev_base, dev_board_test' is included
	#endif
#endif

#endif