/*
	board_pico.h - configuration flags for Raspberry Pi Pico
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

#include "board_flags.h"

// if any supported pico board is selected
#define SUPPORTED_PICO ( \
	defined(CUSTOM_PICO) /* user custom pico */ || \
	defined(RASPBERRYPI_PICO) || \
	defined(ARDUINO_RASPBERRY_PI_PICO) || \
	defined(RASPBERRYPI_PICO_W) || \
	defined(ARDUINO_RASPBERRY_PI_PICO_W) || \
	defined(RASPBERRYPI_PICO2) || \
	defined(ARDUINO_RASPBERRY_PI_PICO2) || \
	defined(RASPBERRYPI_PICO2_W) || \
	defined(ARDUINO_RASPBERRY_PI_PICO2_W) \
)

#if SUPPORTED_PICO

	/****************************
	 * Library Includes
	****************************/

	#include <pico.h>

	/****************************
	 * General Config
	****************************/

	/**
	 * Sets function to run in RAM if possible
	 * 
	 * @param function function name
	 * 
	 * @note {return_type} RUN_IN_RAM({function_name}) {function_name} ({params}) {{content}}
	 */
	#define RUN_IN_RAM(function) __not_in_flash(__STRING( function ))

	/****************************
	 * Serial Config
	****************************/

	#define SERIAL_PRINTF // uses printf as serial

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

#endif

#endif