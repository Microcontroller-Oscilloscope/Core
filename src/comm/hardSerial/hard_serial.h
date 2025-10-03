/*
	hard_serial.h - serial communication for board
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

#ifndef HARD_SERIAL_H
#define HARD_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_FLOAT_PLACES 2
#define DEFAULT_DOUBLE_PLACES 4

#include "../../compile_flags/compile_flags.h"

/**
 * Sets up Serial communication
 * 
 * @param value value to print
 */
void hardPrintBegin(uint32_t baud);

/**
 * Prints memory char array to Serial
 * 
 * @param value value to print
 */
void hardPrintMemCharArray(memCharString* value);
/**
 * Prints char array to Serial
 * 
 * @param value value to print
 */
void hardPrintCharArray(const char* value);
/**
 * Prints int8 to Serial
 * 
 * @param value value to print
 */
void hardPrintInt8(int8_t value);
/**
 * Prints uint8 to Serial
 * 
 * @param value value to print
 */
void hardPrintUInt8(uint8_t value);
/**
 * Prints int16 to Serial
 * 
 * @param value value to print
 */
void hardPrintInt16(int16_t value);
/**
 * Prints uint16 to Serial
 * 
 * @param value value to print
 */
void hardPrintUInt16(uint16_t value);
/**
 * Prints int32 to Serial
 * 
 * @param value value to print
 */
void hardPrintInt32(int32_t value);
/**
 * Prints uint32 to Serial
 * 
 * @param value value to print
 */
void hardPrintUInt32(uint32_t value);
/**
 * Prints int64 to Serial
 * 
 * @param value value to print
 */
void hardPrintInt64(int64_t value);
/**
 * Prints uint64 to Serial
 * 
 * @param value value to print
 */
void hardPrintUInt64(uint64_t value);
/**
 * Prints float to Serial
 * 
 * @param value value to print
 * @param places amount of decimal places to show
 */
void hardPrintFloat(float value, uint8_t places = DEFAULT_FLOAT_PLACES);
/**
 * Prints double to Serial
 * 
 * @param value value to print
 * @param places amount of decimal places to show
 */
void hardPrintDouble(double value, uint8_t places = DEFAULT_DOUBLE_PLACES);

/**
 * Prints memory char array to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintMemCharArrayln(memCharString* value);
/**
 * Prints char array to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintCharArrayln(const char* value);
/**
 * Prints int8 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintInt8ln(int8_t value);
/**
 * Prints uint8 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintUInt8ln(uint8_t value);
/**
 * Prints int16 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintInt16ln(int16_t value);
/**
 * Prints uint16 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintUInt16ln(uint16_t value);
/**
 * Prints int32 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintInt32ln(int32_t value);
/**
 * Prints uint32 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintUInt32ln(uint32_t value);
/**
 * Prints int64 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintInt64ln(int64_t value);
/**
 * Prints uint64 to Serial with termination
 * 
 * @param value value to print
 */
void hardPrintUInt64ln(uint64_t value);
/**
 * Prints float to Serial with termination
 * 
 * @param value value to print
 * @param places amount of decimal places to show
 */
void hardPrintFloatln(float value, uint8_t places = DEFAULT_FLOAT_PLACES);
/**
 * Prints double to Serial with termination
 * 
 * @param value value to print
 * @param places amount of decimal places to show
 */
void hardPrintDoubleln(double value, uint8_t places = DEFAULT_DOUBLE_PLACES);
/**
 * Prints termination to Serial
 * 
 * @param value value to print
 */
void hardPrintln(void);

#ifdef __cplusplus
}
#endif

#ifdef PLATFORMIO
#include "hard_serial_def.h"
#endif

#endif