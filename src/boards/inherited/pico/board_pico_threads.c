/*
	board_pico_threads.h - thread configuration for all Raspberry Pi Picos
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

#if defined(PICO)

#include <hardware/sync.h>
#include <pico/multicore.h>

#include "../../../board_common.h"

bool locked = false;
uint32_t intrruptStatus;

bool startThreadSafety(void) {

	if (!locked) {
		intrruptStatus = save_and_disable_interrupts();
		//multicore_lockout_start_blocking();
		locked = true;
		return true;
	}
	return false;
}

bool endThreadSafety(void) {
	if (locked) {
		//multicore_lockout_end_blocking();
		restore_interrupts(intrruptStatus);
		locked = false;
		return true;
	}
	return false;
}

#endif