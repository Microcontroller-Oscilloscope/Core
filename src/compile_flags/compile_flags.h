/*
	core_pref.h - methods for Preferences usage
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

#ifndef COMPILE_FLAGS_H
#define COMPILE_FLAGS_H

// version of microcontroller oscilloscope core
#define OSC_CORE_MAJOR_VERSION 0
#define OSC_CORE_MINOR_VERSION 3
#define OSC_CORE_PATCH_VERSION 0

#include "../boards/board.h"

/****************************
 * Debug Flags
****************************/

/**
 * Debug flag to show undefined methods
 * Only uncomment when debugging
 */
//#define DEBUG_INSPECT

/**
 * Debug flag to show undefined nvm methods
 * Only uncomment when debugging
 */
//#define DEBUG_NVM

/****************************
 * NVM Config
****************************/

/**
 * Uses EEPROM method for NVM storage
 */
#if defined(UNOR3) || defined(PICO)
	#if DEFAULT_NVM
		#define NVM_EEPROM
	#endif
#endif

/**
 * Uses Preferences method for NVM storage
 */
#if defined(ESP32DEVC)
	#if DEFAULT_NVM
		#define NVM_PREF
	#endif
#endif

/****************************
 * Variable Support
****************************/

/**
 * Supports 64 bit operations natively
 */
#if defined(ESP32DEVC) || defined(PICO)
	#define INT64_SUPPORT
#endif

/**
 * Only compiles char array operations to needed platforms
 */
#if defined(UNOR3)
	#define NO_CHAR_ARRAY_SUPPORT
#endif

/****************************
 * Debug Toggles
****************************/

/**
 * Defines flags to show hidden nvm methods
 */
#ifdef DEBUG_NVM

	#ifndef NVM_EEPROM
		#define NVM_EEPROM
	#endif
	#ifndef NVM_PREF
		#define NVM_PREF
	#endif
	#ifndef DEBUG_INSPECT
		#define DEBUG_INSPECT
	#endif

#endif

/**
 * Defines flags to show all hidden methods
 */
#if defined(DEBUG_INSPECT) && !defined(__TEST_CASES__)

	#ifndef __ERROR_DEBUG__
		#define __ERROR_DEBUG__
	#endif
	#ifndef __NVM_DEBUG__
		#define __NVM_DEBUG__
	#endif

#endif

#endif