/*
	hard_serial_printf.h - serial configuration for standard C out
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

#ifndef HARD_SERIAL_PRINTF_H
#define HARD_SERIAL_PRINTF_H

#include "../../compile_flags/compile_flags.h"

#ifdef SERIAL_PRINTF

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintMemCharArray(memCharString* value) {
	printf(value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintCharArray(const char* value) {
	printf(value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintBool(bool value) {
	printf("%d", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintChar(char value) {
	printf("%c", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt8(int8_t value) {
	printf("%d", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt8(uint8_t value) {
	printf("%u", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt16(int16_t value) {
	printf("%d", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt16(uint16_t value) {
	printf("%u", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt32(int32_t value) {
	printf("%d", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt32(uint32_t value) {
	printf("%u", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt64(int64_t value) {
	printf("%d", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt64(uint64_t value) {
	printf("%u", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintFloat(float value) {
	printf("%f", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintFloatPlaces(float value, uint8_t places) {
	printf("%.*f", places, value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintDouble(double value) {
	printf("%f", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintDoublePlaces(double value, uint8_t places) {
	printf("%.*f", places, value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintMemCharArrayln(memCharString* value) {
	printf(value);
	printf("\n");
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintCharArrayln(const char* value) {
	printf(value);
	printf("\n");
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintBoolln(bool value) {
	printf("%d\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintCharln(char value) {
	printf("%c\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt8ln(int8_t value) {
	printf("%d\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt8ln(uint8_t value) {
	printf("%u\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt16ln(int16_t value) {
	printf("%d\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt16ln(uint16_t value) {
	printf("%u\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt32ln(int32_t value) {
	printf("%d\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt32ln(uint32_t value) {
	printf("%u\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintInt64ln(int64_t value) {
	printf("%d\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintUInt64ln(uint64_t value) {
	printf("%u\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintFloatln(float value) {
	printf("%f\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintFloatPlacesln(float value, uint8_t places) {
	printf("%.*f\n", places, value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintDoubleln(double value) {
	printf("%f\n", value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintDoublePlacesln(double value, uint8_t places) {
	printf("%.*f\n", places, value);
}

#ifdef __cplusplus
extern "C"
#endif
inline void hardPrintln(void) {
	printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif

#endif