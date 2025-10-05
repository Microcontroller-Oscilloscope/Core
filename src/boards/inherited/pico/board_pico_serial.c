/*
	board_esp32_serial.c - serial configuration for Raspberry Pi Picos
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

#include "../../board.h"

#if defined(PICO) && defined(SERIAL_PRINTF)

//#include "../../../comm/hard_serial/hard_serial.h"

#ifdef PLATFORMIO

	void hardPrintBegin(uint32_t baud) {}

#else

	#include <pico/stdio_uart.h>

	void hardPrintBegin(uint32_t baud) {
		uart_init(uart0, BAUD_RATE);
	}

#endif
#endif