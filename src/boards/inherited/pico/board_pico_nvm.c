/*
	board_esp32_nvm.c - nvm configuration for Raspberry Pi Picos
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

#if defined(PICO) && defined(NVM_INTERNAL)

#include "../../../board_common.h"
#include "../../../nvm/generic_nvm.h"
#include "../../../comm/hard_serial/hard_serial.h"

#include <hardware/flash.h>
#include <hardware/sync.h>
#include <pico/multicore.h>

bool nvmBegan = false;
bool chainLock = false; // prevents multiple erease/write of flash

#define SECTOR_SIZE 4096u
#define PAGE_SIZE 256u

#define FLASH_START (uint32_t)PICO_FLASH_SIZE_BYTES - SECTOR_SIZE

uint8_t memoryNVM[SECTOR_SIZE];
nvm_size_t internalSize;

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {

	if (nvmBegan) {
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		return NVM_INVALID_SIZE;
	}

	if (setNVMSize > SECTOR_SIZE) {
		return NVM_INVALID_SIZE;
	}

	internalSize = setNVMSize;

	memcpy((void*)memoryNVM, (const void*)XIP_BASE + FLASH_START, internalSize);

	nvmBegan = true;

	if (!nvmBegan) {
		return NVM_FAILED;
	}

	return NVM_OK;
}

bool nvmMaxSize(nvm_size_t *size) {
	if (nvmBegan) {
		*size = SECTOR_SIZE;
		return true;
	}

	*size = DEFAULT_NVM_SIZE;
	return false;
}

void nvmCommit() {

	if (!chainLock) {

		startThreadSafety();
		flash_range_erase(FLASH_START, SECTOR_SIZE);
		endThreadSafety();
		
		for (int16_t page = 0; page < SECTOR_SIZE / PAGE_SIZE; page++) {
			startThreadSafety();
			flash_range_program(FLASH_START + (page*PAGE_SIZE), (uint8_t*)(memoryNVM + page*PAGE_SIZE), PAGE_SIZE);
			endThreadSafety();
		}
	}
}

enum NVMDefaultCode nvmSetDefaults(void) {
	nvm_size_t nvmMaxValue;
	if (nvmMaxSize(&nvmMaxValue)) {
		if (NVM_SIZE > nvmMaxValue) {
			return NVM_DEFAULT_SIZE_TOO_BIG;
		}
	}
	else {
		// if nvm not started or unable to get size
		return NVM_DEFAULT_FAIL_MAX_SIZE;
	}

	chainLock = true;

	// writes critical values
	enum NVMDefaultCode code = nvmSetCritDefaults(nvmMaxValue);
	if (code != NVM_DEFAULT_OK) {
		chainLock = false;
		return code;
	}

	//writes platform values
	code = nvmSetEnvDefaults();
	if (code != NVM_DEFAULT_OK) {
		chainLock = false;
		return code;
	}

	chainLock = false;
	nvmCommit();
	return code;
}

bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {
	return false;
}

bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength) {
	return false;
}

#define WRITE_NVM(key, value) \
	if (!nvmBegan) { \
		return false; \
	} \
	if (key > internalSize) { \
		return false; \
	} \
	for (uint8_t i = 0; i < sizeof(value); i++) { \
		memoryNVM[key + i] = (uint8_t)(value >> ((sizeof(value) - i - 1) * 8)); \
	} \
	nvmCommit(); \
	return true;

#define GET_NVM(key, value, type) \
	*value = 0; \
	if (!nvmBegan) { \
		return false; \
	} \
	if (key > internalSize) { \
		return false; \
	} \
	for (uint8_t i = 0; i < sizeof(*value); i++) { \
		*value |= ((type)memoryNVM[key + i]) << ((sizeof(*value) - i - 1) * 8); \
	} \
	return true; \

bool nvmWriteBool(nvm_size_t key, bool value) {

	WRITE_NVM(key, value);
}

bool nvmWriteI8(nvm_size_t key, int8_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteI16(nvm_size_t key, int16_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteI32(nvm_size_t key, int32_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteI64(nvm_size_t key, int64_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	WRITE_NVM(key, value);
}

bool nvmWriteFloat(nvm_size_t key, float value) {
	return false;
}

bool nvmWriteDouble(nvm_size_t key, double value) {
	return false;
}

bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	GET_NVM(key, value, bool);
}

bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	GET_NVM(key, value, int8_t);
}

bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	GET_NVM(key, value, uint8_t);
}

bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	GET_NVM(key, value, int16_t);
}

bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	GET_NVM(key, value, uint16_t);
}

bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	GET_NVM(key, value, int32_t);
}

bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	GET_NVM(key, value, uint32_t);
}

bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	GET_NVM(key, value, int64_t);
}

bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	GET_NVM(key, value, uint64_t);
}

bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {
	return false;
}

bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {
	return false;
}

#endif