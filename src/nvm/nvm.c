/*
	nvm.c - methods for generic nvm usage
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

#include "nvm.h"

#include "eeprom_addresses.h"
#include "../osc_err/osc_err.h"
#include "../comm/hard_serial/hard_serial.h"

#include <stddef.h>

#ifndef NO_CHAR_ARRAY_SUPPORT

	bool sameString(char* first, char* second) {
		uint8_t firstSize = charArraySize(first);
		uint8_t secondSize = charArraySize(second);

		if (firstSize != secondSize || firstSize == 0) {
			return false;
		}

		for (uint8_t i = 0; i < firstSize; i++) {
			if (first[i] != second[i]) {
				return false;
			}
		}

		return true;
	}

#endif

bool validCharPointer(const char* value) {
	if (value == NULL) {
		return false;
	}
	return true;
}

memCharString failWriteStr[] = {"nvm failed write"};

enum NVMDefaultCode nvmSetCritDefaults(nvm_size_t nvmMaxValue) {

	if (sizeof(nvm_size_t) == sizeof(uint8_t)) {
		if (!nvmWriteUI8(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint16_t)) {
		if (!nvmWriteUI16(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint32_t)) {
		if (!nvmWriteUI32(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint64_t)) {
		if (!nvmWriteUI64(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else {
		return NVM_DEFAULT_FAIL_WRITE;
	}

	return NVM_DEFAULT_OK;
}

enum NVMDefaultCode nvmSetEnvDefaults(void) {
	return NVM_DEFAULT_OK;
}