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

/****************************
 * Debug Flags
****************************/

/**
 * Debug flag to show undefined methods
 */
//#define DEBUG_INSPECT

/**
 * Debug flag to show undefined nvm methods
 */
//#define DEBUG_NVM

/****************************
 * Selected Board
****************************/

/**
 * Arduino Uno r3
 */
#ifdef ARDUINO_AVR_UNO
#define UNOR3
#endif

/**
 * ESP32 Devkit C v4
 */
#ifdef ESP32
#define ESP32DEVC
#endif

/**
 * Raspberry Pi Pico
 */
#ifdef ARDUINO_RASPBERRY_PI_PICO
#define PICO
#endif

/****************************
 * EEPROM Config
****************************/

/**
 * Uses EEPROM method for NVM storage
 */
#if defined(UNOR3) || defined(PICO)
#define NVM_EEPROM
#endif

/**
 * Uses Preferences method for NVM storage
 */
#if defined(ESP32DEVC)
#define NVM_PREF
#endif

/****************************
 * Variable Support
****************************/

#if defined(ESP32DEVC) || defined(PICO)
#define INT64_SUPPORT
#endif

/****************************
 * Debug Toggles
****************************/

/**
 * Defines flags to show hidden nvm methods
 */
#ifdef DEBUG_NVM

#define __NVM_BEGIN__
#define __NVM_BEGIN_SIZE__
#define __NVM_COMMIT__
#undef INT64_SUPPORT
#define DEBUG_INSPECT

#endif

/**
 * Defines flags to show all hidden methods
 */
#ifdef DEBUG_INSPECT

#undef NVM_PREF
#undef NVM_EEPROM
#define __ERROR_DEBUG__
#define __NVM_DEBUG__
#define __TAG_DEBUG__

#endif