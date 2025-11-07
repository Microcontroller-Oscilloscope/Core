/*
	board_flags.h - configuration flags for all boards
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

#ifndef BOARD_FLAGS_H
#define BOARD_FLAGS_H

/**
 * Macro for contatenating flags as numbers
 * 
 * @param a first input
 * @param b second input
 * 
 * @return contatenated value
 * 
 * @note Example:
 * @note #define NUMBER 1
 * @note #define FUNCTION(id) CONCATENATE(function, id)
 * @note FUNCTION(NUMBER)
 * @note //returns 'function1' instead of 'functionNUMBER'
 */
#define CONCATENATE(a, b) a ## b

/**
 * Macro for contatenating 3 flags as numbers
 * 
 * @param a first input
 * @param b second input
 * @param c third input
 * 
 * @return contatenated value
 * 
 * @note Example:
 * @note #define NUMBER 1
 * @note #define FUNCTION(id) CONCATENATE(function, id, name)
 * @note FUNCTION(NUMBER)
 * @note //returns 'function1name' instead of 'functionNUMBERname'
 */
#define CONCATENATE3(a, b, c) a ## b ## c

/**
 * Converts bit to bit mask
 * 
 * @param bit bit to shift to
 */
#define TO_BIT(bit) (1 << (bit))

// default nvm size when flash is used instead of dedicated EEPROM
#define FLASH_NVM_SIZE 4096

/****************************
 * Common Types
****************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

typedef const char memCharString;

/****************************
 * Changes Flags When Testing
****************************/

#ifndef __TEST_CASES__
	#if defined(PIO_UNIT_TESTING) || defined(UNIT_TEST)
		#define __TEST_CASES__ // indicator if compiling for testing
	#endif
#endif

/****************************
 * Checks Custom Config Flags
****************************/

#ifndef WAIT_RUN
	#define WAIT_RUN 0 // how long to wait for program to run in ms
#endif

#ifndef DEFAULT_BAUD_RATE
	#define DEFAULT_BAUD_RATE 9600 // default baud rate for microcontrollers
#endif

// sets function as weak with c attribute
#define WEAK __attribute__((weak))

#endif