/*
	board_pico1.h - configuration flags for Raspberry Pi Pico
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

#ifndef BOARD_PICO1_H
#define BOARD_PICO1_H

#include "../board_generic.h"

#ifdef ARDUINO_RASPBERRY_PI_PICO
	#define PICO1
	#define PICO
	#define BOARD_FOUND
#endif

#ifdef PICO1

	#include "../inherited/pico/board_pico.h"

#endif
#endif