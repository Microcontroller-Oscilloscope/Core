/*
	hard_serial_def.h - defines serial communication for board
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

#ifndef HARD_SERIAL_DEF_H
#define HARD_SERIAL_DEF_H

#ifdef PLATFORMIO
#include <Arduino.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern "C" inline void hardPrintBegin(uint32_t baud) {
	Serial.begin(baud);
}

extern "C" inline void hardPrintCharArray(const char* value) {
	Serial.print(value);
}

extern "C" inline void hardPrintInt8(int8_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintUInt8(uint8_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintInt16(int16_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintUInt16(uint16_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintInt32(int32_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintUInt32(uint32_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintFloat(float value, uint8_t places) {
	Serial.print(value, places);
}

extern "C" inline void hardPrintDouble(double value, uint8_t places) {
	Serial.print(value, places);
}

extern "C" inline void hardPrintCharArrayln(const char* value) {
	Serial.println(value);
}

extern "C" inline void hardPrintInt8ln(int8_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintUInt8ln(uint8_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintInt16ln(int16_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintUInt16ln(uint16_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintInt32ln(int32_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintUInt32ln(uint32_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintFloatln(float value, uint8_t places) {
	Serial.println(value, places);
}

extern "C" inline void hardPrintDoubleln(double value, uint8_t places) {
	Serial.println(value, places);
}

extern "C" inline void hardPrintln(void) {
	Serial.println();
}

#ifndef NO_PROGMEM_COPY_SUPPORT

extern "C" inline void hardPrintMemCharArray(memCharString* value) {
	Serial.print(value);
}

extern "C" inline void hardPrintMemCharArrayln(memCharString* value) {
	Serial.println(value);
}

#endif

#ifndef INT64_SUPPORT

extern "C" inline void hardPrintInt64ln(int64_t value) {
	hardPrintInt64(value);
	Serial.println();
}

extern "C" inline void hardPrintUInt64ln(uint64_t value) {
	hardPrintUInt64(value);
	Serial.println();
}

#else

extern "C" inline void hardPrintInt64(int64_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintUInt64(uint64_t value) {
	Serial.print(value);
}

extern "C" inline void hardPrintInt64ln(int64_t value) {
	Serial.println(value);
}

extern "C" inline void hardPrintUInt64ln(uint64_t value) {
	Serial.println(value);
}

#endif

#ifdef __cplusplus
}
#endif

#endif