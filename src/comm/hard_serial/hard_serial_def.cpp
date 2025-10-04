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

#ifdef PLATFORMIO
#include <Arduino.h>
#endif

extern "C" void hardPrintBegin(uint32_t baud) {
	Serial.begin(baud);
}

extern "C" void hardPrintCharArray(const char* value) {
	Serial.print(value);
}

extern "C" void hardPrintBool(bool value) {
	Serial.print(value);
}

extern "C" void hardPrintChar(char value) {
	Serial.print(value);
}

extern "C" void hardPrintInt8(int8_t value) {
	Serial.print(value);
}

extern "C" void hardPrintUInt8(uint8_t value) {
	Serial.print(value);
}

extern "C" void hardPrintInt16(int16_t value) {
	Serial.print(value);
}

extern "C" void hardPrintUInt16(uint16_t value) {
	Serial.print(value);
}

extern "C" void hardPrintInt32(int32_t value) {
	Serial.print(value);
}

extern "C" void hardPrintUInt32(uint32_t value) {
	Serial.print(value);
}

extern "C" void hardPrintFloat(float value) {
	Serial.print(value, DEFAULT_FLOAT_PLACES);
}

extern "C" void hardPrintFloatPlaces(float value, uint8_t places) {
	Serial.print(value, places);
}

extern "C" void hardPrintDouble(double value) {
	Serial.print(value, DEFAULT_DOUBLE_PLACES);
}

extern "C" void hardPrintDoublePlaces(double value, uint8_t places) {
	Serial.print(value, places);
}

extern "C" void hardPrintCharArrayln(const char* value) {
	Serial.println(value);
}

extern "C" void hardPrintBoolln(bool value) {
	Serial.println(value);
}

extern "C" void hardPrintCharln(char value) {
	Serial.print(value);
}

extern "C" void hardPrintInt8ln(int8_t value) {
	Serial.println(value);
}

extern "C" void hardPrintUInt8ln(uint8_t value) {
	Serial.println(value);
}

extern "C" void hardPrintInt16ln(int16_t value) {
	Serial.println(value);
}

extern "C" void hardPrintUInt16ln(uint16_t value) {
	Serial.println(value);
}

extern "C" void hardPrintInt32ln(int32_t value) {
	Serial.println(value);
}

extern "C" void hardPrintUInt32ln(uint32_t value) {
	Serial.println(value);
}

extern "C" void hardPrintFloatln(float value) {
	Serial.println(value, DEFAULT_FLOAT_PLACES);
}

extern "C" void hardPrintFloatPlacesln(float value, uint8_t places) {
	Serial.println(value, places);
}

extern "C" void hardPrintDoubleln(double value) {
	Serial.println(value, DEFAULT_DOUBLE_PLACES);
}

extern "C" void hardPrintDoublePlacesln(double value, uint8_t places) {
	Serial.println(value, places);
}

extern "C" void hardPrintln(void) {
	Serial.println();
}

#ifndef NO_PROGMEM_COPY_SUPPORT

extern "C" void hardPrintMemCharArray(memCharString* value) {
	Serial.print(value);
}

extern "C" void hardPrintMemCharArrayln(memCharString* value) {
	Serial.println(value);
}

#else

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

extern "C" void hardPrintInt64ln(int64_t value) {
	hardPrintInt64(value);
	Serial.println();
}

extern "C" void hardPrintUInt64ln(uint64_t value) {
	hardPrintUInt64(value);
	Serial.println();
}

#else

extern "C" void hardPrintInt64(int64_t value) {
	Serial.print(value);
}

extern "C" void hardPrintUInt64(uint64_t value) {
	Serial.print(value);
}

extern "C" void hardPrintInt64ln(int64_t value) {
	Serial.println(value);
}

extern "C" void hardPrintUInt64ln(uint64_t value) {
	Serial.println(value);
}

#endif