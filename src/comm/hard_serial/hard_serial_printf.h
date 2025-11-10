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

#if SUPPORTED_ESP32
	#include <esp_idf_version.h>
	#if ESP_IDF_VERSION_MAJOR == 5
		#define PRINT_LONG_SPECIFIER
	#endif
#endif

#if defined(SERIAL_PRINTF)

#ifdef __cplusplus
extern "C" {
#endif

static inline void hardPrintMemCharArray(memCharString* value) {
	printf(value);
}

static inline void hardPrintCharArray(const char* value) {
	printf(value);
}

static inline void hardPrintBool(bool value) {
	printf("%d", value);
}

static inline void hardPrintChar(char value) {
	printf("%c", value);
}

static inline void hardPrintInt8(int8_t value) {
	printf("%d", value);
}

static inline void hardPrintUInt8(uint8_t value) {
	printf("%u", value);
}

static inline void hardPrintInt16(int16_t value) {
	printf("%d", value);
}

static inline void hardPrintUInt16(uint16_t value) {
	printf("%u", value);
}

static inline void hardPrintInt32(int32_t value) {
	#ifdef PRINT_LONG_SPECIFIER
		printf("%ld", value);
	#else
		printf("%d", value);
	#endif
}

static inline void hardPrintUInt32(uint32_t value) {
	#ifdef PRINT_LONG_SPECIFIER
		printf("%lu", value);
	#else
		printf("%u", value);
	#endif
}

static inline void hardPrintInt64(int64_t value) {
	printf("%lld", value);
}

static inline void hardPrintUInt64(uint64_t value) {
	printf("%llu", value);
}

static inline void hardPrintFloat(float value) {
	printf("%f", (double)value);
}

static inline void hardPrintFloatPlaces(float value, uint8_t places) {
	printf("%.*f", places, (double)value);
}

static inline void hardPrintDouble(double value) {
	printf("%f", value);
}

static inline void hardPrintDoublePlaces(double value, uint8_t places) {
	printf("%.*f", places, value);
}

static inline void hardPrintMemCharArrayln(memCharString* value) {
	printf(value);
	printf("\n");
}

static inline void hardPrintCharArrayln(const char* value) {
	printf(value);
	printf("\n");
}

static inline void hardPrintBoolln(bool value) {
	printf("%d\n", value);
}

static inline void hardPrintCharln(char value) {
	printf("%c\n", value);
}

static inline void hardPrintInt8ln(int8_t value) {
	printf("%d\n", value);
}

static inline void hardPrintUInt8ln(uint8_t value) {
	printf("%u\n", value);
}

static inline void hardPrintInt16ln(int16_t value) {
	printf("%d\n", value);
}

static inline void hardPrintUInt16ln(uint16_t value) {
	printf("%u\n", value);
}

static inline void hardPrintInt32ln(int32_t value) {
	#ifdef PRINT_LONG_SPECIFIER
		printf("%ld\n", value);
	#else
		printf("%d\n", value);
	#endif
}

static inline void hardPrintUInt32ln(uint32_t value) {
	#ifdef PRINT_LONG_SPECIFIER
		printf("%lu\n", value);
	#else
		printf("%u\n", value);
	#endif
}

static inline void hardPrintInt64ln(int64_t value) {
	printf("%lld\n", value);
}

static inline void hardPrintUInt64ln(uint64_t value) {
	printf("%llu\n", value);
}

static inline void hardPrintFloatln(float value) {
	printf("%f\n", (double)value);
}

static inline void hardPrintFloatPlacesln(float value, uint8_t places) {
	printf("%.*f\n", places, (double)value);
}

static inline void hardPrintDoubleln(double value) {
	printf("%f\n", value);
}

static inline void hardPrintDoublePlacesln(double value, uint8_t places) {
	printf("%.*f\n", places, value);
}

static inline void hardPrintln(void) {
	printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif

#endif