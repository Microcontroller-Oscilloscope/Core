/*
	hard_serial.c - serial communication for board
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

#include "hard_serial.h"

uint8_t charArraySize(const char* value) {

	uint8_t max = CHAR_LEN_ERROR - 1;
	for (uint8_t i = 0U; i < max; i++) {
		if (!(value[i] ^ END_OF_CHAR)) {
			return i + 1;
		}
	}

	return CHAR_LEN_ERROR;
}