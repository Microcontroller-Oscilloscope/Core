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

#include "compile_flags/compile_flags.h"
#include <WString.h>

/**
 * Prints formatted tag to serial monitor '[tag]:'
 * 
 * @param tag tag formatted with F({String})
 * @return void
 */
void printTag(const __FlashStringHelper * tag);

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

/**
 * Prints critical error tag '[!!!]:'
 */
void printCritError(void);

#ifndef INT64_SUPPORT

#include <Arduino.h>

/**
 * Converts to n power of input datatypes
 * 
 * @param base base of exponent
 * @param power power of exponent
 * 
 * @return final value
 */
template <typename T>
uint64_t toNPower(T base, T power) {
	uint64_t value = 1;
	for (T i = 0; i < power; i++) {
		value *= base;
	}
	return value;
}

/**
 * Prints a 64 bit integer to the serial monitor
 * 
 * @param value integer to print
 */
template <typename T>
void printInt64(T value) {

	// prints 0
	if (value == 0) {
		Serial.print("0");
		return;
	}

	if (value < 0) {
		Serial.print(F("-"));
		value *= -1;

		// prints I64 min
		if (value < 0) {
			Serial.print(F("-9223372036854775808"));
			return;
		}
	}

	int8_t digits = 0;
	uint64_t temp = (uint64_t)value;

	// gets count of digits
	while (temp > 0) {
		digits++;
		temp = temp/10U;
	}

	// prints digits
	for (int8_t i = digits - 1; i >= 0; i--) {
		uint64_t power = toNPower((uint8_t)10, (uint8_t)i);
		uint64_t leftover = value % power;
		Serial.print((uint8_t)((value - leftover) / power));
		value -= value - leftover;
	}
}

#endif
#endif