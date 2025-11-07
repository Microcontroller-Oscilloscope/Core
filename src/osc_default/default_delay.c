/*
	default_delay.c - delay configuration for Arduino
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

#include <Arduino.h>

#include "../osc_common/common_thread.h"

void hardDelayMS(uint32_t delayAmount) {
	delay(delayAmount);
}

void hardDelayUS(uint32_t delayAmount) {
	delayMicroseconds(delayAmount);
}

#endif