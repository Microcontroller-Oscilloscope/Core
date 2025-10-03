/*
	hard_serial_def.cpp - defines serial communication for board
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

#include "hard_serial.h"

#if defined(NO_PROGMEM_COPY_SUPPORT) || !defined(INT64_SUPPORT)

	#ifdef PLATFORMIO
	#include <Arduino.h>
	#endif

	#ifdef __cplusplus
	extern "C" {
	#endif

	#ifdef NO_PROGMEM_COPY_SUPPORT

	/**
	 * Copies flash message to serial
	 * 
	 * @param message flash message to print
	 * @param messageSize size of message to print
	 */
	extern "C" void copyMessage(memCharString *message, uint8_t messageSize) {
		for (uint8_t i = 0; i < messageSize; i++) {
			Serial.print((char)pgm_read_byte_near(message + i));
		}
	}

	extern "C" void hardPrintMemCharArray(memCharString* value) {
		copyMessage(value, strlen_P(value));
	}

	extern "C" void hardPrintMemCharArrayln(memCharString* value) {
		copyMessage(value, strlen_P(value));
		Serial.println();
	}

	#endif

	#ifndef INT64_SUPPORT

	/**
	 * Converts to n power of input datatypes
	 * 
	 * @param base base of exponent
	 * @param power power of exponent
	 * 
	 * @return final value
	 */
	extern "C" uint64_t toNPower(uint8_t base, uint8_t power) {
		uint64_t value = 1;
		for (uint8_t i = 0; i < power; i++) {
			value *= base;
		}
		return value;
	}

	extern "C" void hardPrintInt64(int64_t value) {

		if (value == 0) {
			Serial.print("0");
			return;
		}

		Serial.print(F("-"));
		value *= -1;

		// prints I64 min
		if (value < 0) {
			Serial.print(F("-9223372036854775808"));
			return;
		}

		hardPrintUInt64(value);
	}

	extern "C" void hardPrintUInt64(uint64_t value) {

		// prints 0
		if (value == 0) {
			Serial.print("0");
			return;
		}

		int8_t digits = 0;
		uint64_t temp = value;

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

	#ifdef __cplusplus
	}
	#endif

#endif