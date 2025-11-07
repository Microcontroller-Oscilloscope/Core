/*
	board_common.h - configuration flags for all boards
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

#ifndef BOARD_COMMON_H
#define BOARD_COMMON_H

#include "board_avr.h"
#include "board_esp32.h"
#include "board_pico.h"

// if any supported board is selected
#define SUPPORTED_PLATFORM ( \
	SUPPORTED_AVR || \
	SUPPORTED_PICO || \
	SUPPORTED_ESP32 \
)

// if board should use arduino methods
#define ENVIRONMENT_ARDUINO ( \
	!SUPPORTED_PLATFORM \
)

#ifndef RUN_IN_RAM
	/**
	 * Sets function to run in RAM if possible
	 * 
	 * @param function function name
	 * 
	 * @note {return_type} RUN_IN_RAM({function_name}) {function_name} ({params}) {{content}}
	 */
	#define RUN_IN_RAM(function)
#endif

#ifndef PROG_FLASH
	#define PROG_FLASH // storage specifier for flash space
#endif

#ifndef BAUD_RATE
	#define BAUD_RATE DEFAULT_BAUD_RATE
#endif

#endif