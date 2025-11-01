/*
	board_avr.h - configuration flags for Atmel AVR boards
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

#ifndef BOARD_AVR_H
#define BOARD_AVR_H

#include "board_flags.h"

// if any supported avr board is selected
#define SUPPORTED_AVR ( \
	defined(CUSTOM_AVR) /* user custom avr */ || \
	defined(__AVR_ATmega328P__) /* also Arduino Uno */ \
)

#if SUPPORTED_AVR

	/****************************
	 * Library Includes
	****************************/

	#if SUPPORTED_AVR
		#include <avr/pgmspace.h>
		#define PROG_FLASH PROGMEM // storage specifier for flash space
	#endif

	/****************************
	 * General Config
	****************************/

	/****************************
	 * Serial Config
	****************************/

	#ifndef SERIAL_INTERNAL
		#define SERIAL_INTERNAL // uses boards internal functions for transmission
	#endif

	#ifdef SERIAL_INTERNAL
		#ifndef NO_FLOAT_PRINT_SUPPORT
			#define NO_FLOAT_PRINT_SUPPORT
		#endif
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 3 hardware timers available
	****************************/

	#define FREQ_MAX 1000000 // max frequency user set timer can be

	#ifndef NUM_TIMERS
		#define NUM_TIMERS 3 // amount of hardware timers to use
	#endif

#endif
#endif