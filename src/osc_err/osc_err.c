/*
	osc_err.c - handles critical errors
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

#include "osc_err.h"

#include <Arduino.h>
#include "../status/debug.h"
#include "../comm/hard_serial/hard_serial.h"

void errorLoop(enum OSC_ERR err) {
	#ifndef __TEST_CASES__
		while(true) {
			printCritError();
			hardPrintUInt16ln(err);
			delay(OSC_ERR_DELAY);
		}
	#endif
}