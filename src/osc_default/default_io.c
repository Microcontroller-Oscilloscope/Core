/*
	default_io.c - common functions for microcontrollers
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

#include "../compile_flags/compile_flags.h"

#if ENVIRONMENT_ARDUINO

#include "../osc_common/common_io.h"
#include "../osc_common/common_thread.h"
#include "../status/status.h"

#include <Arduino.h>

bool initBoard() {
	return true;
}

void hardPinMode(pin_t pin, enum pinModeState mode) {

	if (mode == PIN_MODE_INPUT) {
		pinMode(pin, INPUT);
	}
	else if (mode == PIN_MODE_OUTPUT) {
		pinMode(pin, OUTPUT);
	}
	else if (mode == PIN_MODE_INPUT_PULL_UP) {
		pinMode(pin, INPUT_PULLUP);
	}
}

void hardDigitalWrite(pin_t pin, uint8_t value) {
	digitalWrite(pin, value);
}

bool getStatusPin(pin_t *pin, enum statusPin status) {

	*pin = PIN_T_INVALID;
	return true;
}

#endif