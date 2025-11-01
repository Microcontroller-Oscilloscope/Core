/*
	entry.c - entry point for code
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

#include "entry.h"

#include "compile_flags/compile_flags.h"
#include "nvm/nvm.h"
#include "osc_err/osc_err.h"
#include "status/status.h"
#include "osc_common.h"
#include "comm/hard_serial/hard_serial.h"

void startProgram(void) {

	// starts serial monitor
	hardPrintBegin(BAUD_RATE);

	hardDelayMS(WAIT_RUN);

	/*if (nvmInit(NVM_SIZE) != NVM_OK) {
		errorLoop(NVM_INIT_FAIL);
	}*/

	/*if (!nvmWriteValue(5, false)) {
		errorLoop(NVM_WRITE_FAIL);
	}

	bool result;
	if (!nvmGetValue(5, &result)) {
		errorLoop(NVM_GET_FAIL);
	}

	if (!nvmGetValue(5, &result, CAN_NOT_DEFAULT)) {
		errorLoop(NVM_GET_FAIL);
	}*/
}