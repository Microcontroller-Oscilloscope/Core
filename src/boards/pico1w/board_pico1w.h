/*
	board_pico1w.h - configuration flags for Raspberry Pi Pico W
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

#ifndef BOARD_PICO1W_H
#define BOARD_PICO1W_H

#include "../board_generic.h"

#ifdef ARDUINO_RASPBERRY_PI_PICO_W
	#define PICO1W
	#define PICO
	#define BOARD_FOUND
#endif

#ifdef PICO1W

	#include "board_pico1w_pins.h"

	#include "../inherited/pico/board_pico.h"

	#include <pins_arduino.h>

	#ifndef EXTERNAL_LED_PIN
		#define EXTERNAL_STATUS_LED_PIN D22 // pin for external status LED
	#endif

	#ifndef STATUS_LED_PIN
		#define STATUS_LED_PIN CYW43_WL_GPIO_LED_PIN // pin for status LED
	#endif

	#ifndef IO_INTERNAL
		#define IO_INTERNAL // uses internal IO functions to set pins
	#endif

	#ifndef DELAY_INTERNAL
		#define DELAY_INTERNAL // uses internal delay functions
	#endif

#endif
#endif