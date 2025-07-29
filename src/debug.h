/*
	debug.h - debug statement formatter
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

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "compile_flags.h"

#ifdef __TAG_DEBUG__
/**
 * Prints formatted tag to serial monitor '[tag]:'
 * 
 * @param tag tag formatted with F()
 * @return void
 */
void printTag(const __FlashStringHelper * tag);
#endif

#ifdef __ERROR_DEBUG__
/**
 * Prints error tag '[Err]:'
 * 
 * @return void
 */
void printError(void);
#endif

#ifdef __NVM_DEBUG__
/**
 * Prints nvm tag '[NVM]:'
 * 
 * @return void
 */
void printNVM(void);
#endif

#ifndef INT64_SUPPORT

/**
 * Prints a 64 bit integer to the serial monitor
 * 
 * @param value integer to print
 */
template <typename T>
void printInt64(T value) {

	bool sign = false;

	if (value < 0) {
		sign = true;
	}

	if (value == 0) {
		Serial.print(F("0"));
		return;
	}

	String final = "";

	while (value) {
		int64_t digit = value % 10;
		int8_t sdigit = abs(digit);
		final = (String)sdigit + final;
		value = (value - digit) / 10;
	}

	if (sign) Serial.print(F("-"));
	Serial.print(final);
}

#endif

#endif