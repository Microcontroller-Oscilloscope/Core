/*
	common_io.h - common io functions for microcontrollers
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

#ifndef COMMON_IO_H
#define COMMON_IO_H

#include "../compile_flags/compile_flags.h"

// datatype for selecting pin
typedef uint8_t pin_t;

// states for digital writes
enum digitalState {
	DIGITAL_LOW,
	DIGITAL_HIGH
};

// states for pin modes
enum pinModeState {
	PIN_MODE_DISABLED,
	PIN_MODE_OUTPUT,
	PIN_MODE_INPUT,
	//PIN_MODE_INPUT_PULL_DOWN,
	PIN_MODE_INPUT_PULL_UP,
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sets pin mode for board
 * 
 * @param pin pin to configure
 * @param mode mode to operate in
 */
void hardPinMode(pin_t pin, enum pinModeState mode);

/**
 * Sets digital pin state
 * 
 * @param pin pin to set
 * @param value value to set
 */
void hardDigitalWrite(pin_t pin, enum digitalState value);

#ifdef __cplusplus
}
#endif

#endif