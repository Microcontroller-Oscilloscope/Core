/*
	board_common.cpp - common functions for microcontrollers
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

#include "board_common.h"

#include <Arduino.h>

extern "C" void hardDelayMS(uint32_t delayAmount) {
	delay(delayAmount);
}

extern "C" void hardDelayUS(uint32_t delayAmount) {
	delayMicroseconds(delayAmount);
}

#ifndef IO_INTERNAL

extern "C" bool initBoard() {
	return true;
}

extern "C" void hardPinMode(pin_t pin, pinModeState mode) {
	pinMode(pin, mode);

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

extern "C" void hardDigitalWrite(pin_t pin, enum digitalState value) {
	digitalWrite(pin, value);
}

#endif