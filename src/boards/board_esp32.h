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

#include "board_flags.h"

// if any supported esp32 board is selected
#define SUPPORTED_ESP32 ( \
	defined(CUSTOM_ESP32) /* user custom esp32 */ || \
	defined(ESP32) \
)

#if SUPPORTED_ESP32

	/****************************
	 * Library Includes
	****************************/

	#include <esp_attr.h>

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
	#define RUN_IN_RAM(function) IRAM_ATTR

	/****************************
	 * Serial Config
	****************************/

	#define SERIAL_PRINTF // uses printf as serial

	#if defined(CONFIG_ESPTOOLPY_MONITOR_BAUD) && !defined(PLATFORMIO)
		#ifdef BAUD_RATE
			#undef BAUD_RATE
		#endif
		#define BAUD_RATE CONFIG_ESPTOOLPY_MONITOR_BAUD
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 4 hardware timers available
	****************************/

	#define FREQ_MAX 5000000 // max frequency user set timer can be

	#ifndef NUM_TIMERS
		#define NUM_TIMERS 4 // amount of hardware timers to use
	#endif

#endif

#endif