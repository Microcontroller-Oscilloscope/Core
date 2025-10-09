/*
	board_uno_io.c - IO configuration for Raspberry Pi Pico W
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

#include "../board.h"

#if defined(PICO1W) && defined(IO_INTERNAL)

#include "../../board_common.h"

#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

bool initBoard() {

	//stdio_init_all();
	//cyw43_arch_init();

	return true;
}

void hardPinMode(pin_t pin, enum pinModeState mode) {
	if (mode == PIN_MODE_DISABLED) {
		gpio_deinit(pin);
	}
	else if (mode == PIN_MODE_OUTPUT) {
		gpio_init(pin);
		gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_4MA);
		gpio_set_dir(pin, GPIO_OUT);
	}
	else if (mode == PIN_MODE_INPUT) {
		gpio_init(pin);
		gpio_set_dir(pin, GPIO_IN);
		gpio_disable_pulls(pin);
	}
	else if (mode == PIN_MODE_INPUT_PULL_UP) {
		gpio_init(pin);
		gpio_set_dir(pin, GPIO_IN);
		gpio_pull_up(pin);
		gpio_put(pin, 0);
	}
}

void hardDigitalWrite(pin_t pin, enum digitalState value) {

	if (pin == STATUS_LED_PIN) {
		cyw43_arch_gpio_put(pin, value);
	}
	else {
		gpio_put(pin, value);
	}
}

#endif