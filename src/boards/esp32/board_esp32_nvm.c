/*
	board_esp32_nvm.c - nvm configuration for Espressif ESP32
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

#if defined(ESP32DEVC) && defined(NVM_INTERNAL)

#include "../../board_common.h"
#include "../../nvm/generic_nvm.h"

#include <nvs.h>
#include <nvs_flash.h>

#define CHAR_KEY_SIZE NVM_MAX_SIZE_BYTES + 1U
#define OSC_NAME_SPACE "Osc"

bool nvmBegan = false;
nvs_handle_t handler;

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (nvmBegan) {
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		return NVM_INVALID_SIZE;
	}

	if (nvs_flash_init() != ESP_OK) {
		return NVM_FAILED;
	}

	if (nvs_open(OSC_NAME_SPACE, NVS_READWRITE, &handler) != ESP_OK) {
		return NVM_FAILED;
	}

	nvmBegan = true;

	return NVM_OK;
}

bool nvmMaxSize(nvm_size_t *size) {
	if (nvmBegan) {
		*size = NVM_MAX_SIZE;
		return true;
	}

	*size = DEFAULT_NVM_SIZE;
	return false;
}

/**
 * Stops and closes NVM
 * 
 * @warning INTENDED TO USE AFTER 'nvmSetDefaults'
 * @warning NOT INTENDED FOR EXTERNAL USE
 * 
 * @note requires 'nvmInit' after calling
 * 
 * @return if stop was successful
 */
bool nvmStop(void) {
	if (!nvmBegan) {
		return false;
	}

	nvs_flash_deinit();
	nvmBegan = false;

	return true;
}

/**
 * Clears all nvm data
 * 
 * @warning WILL CLEAR NVM DATA
 * @warning NOT INTENDED FOR EXTERNAL USE
 * 
 * @return if clear was successful
 */
bool nvmClear(void) {

	if (nvs_flash_erase() != ESP_OK) {
		return false;
	}
	if (nvs_flash_init() != ESP_OK) {
		return false;
	}

	return true;
}

enum NVMDefaultCode nvmSetDefaults(void) {
	// ensures NVM_SIZE isn't too big for microcontroller
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

	// ensures clear works
	if (!nvmClear()) {
		return NVM_DEFAULT_FAIL_CLEAR;
	}

	// stops nvm
	if (!nvmStop()) {
		return NVM_DEFAULT_FAIL_STOP;
	}

	// restarts nvm for operations
	enum NVMStartCode startCode = nvmInit(NVM_SIZE);
	if (startCode != NVM_OK) {
		return NVM_DEFAULT_FAIL_INIT;
	}

	// writes critical values
	enum NVMDefaultCode code = nvmSetCritDefaults(nvmMaxValue);
	if (code != NVM_DEFAULT_OK) {
		return code;
	}

	//writes platform values
	return nvmSetEnvDefaults();
}

/**
 * Converts integer key to char array key
 * 
 * @param key integer key
 * @param keyStr char array to store key
 */
void keyToChar(nvm_size_t key, char* keyStr) {
	for (uint8_t i = 0U; i < NVM_MAX_SIZE_BYTES; i++) {
		uint8_t subKey = (key >> (8*i)) & (ONE_BYTE);
		keyStr[i] = subKey;
	}
	keyStr[NVM_MAX_SIZE_BYTES] = END_OF_CHAR;
}

#define SET_NVS(key, setter) \
	if (!nvmBegan) { \
		return false; \
	} \
	char keyStr[CHAR_KEY_SIZE]; \
	keyToChar(key, keyStr); \
	if (setter(handler, keyStr, value) != ESP_OK) { \
		return false; \
	} \
	if (nvs_commit(handler) != ESP_OK) { \
		return false; \
	} \
	return true;

#define GET_NVS(key, getter, value) \
	if (!nvmBegan) { \
		return false; \
	} \
	char keyStr[CHAR_KEY_SIZE]; \
	keyToChar(key, keyStr); \
	if (getter(handler, keyStr, value) != ESP_OK) { \
		return false; \
	} \
	return true;

bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {}

bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength) {}

bool nvmWriteBool(nvm_size_t key, bool value) {
	SET_NVS(key, nvs_set_u8);
}

bool nvmWriteI8(nvm_size_t key, int8_t value) {
	SET_NVS(key, nvs_set_i8);
}

bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	SET_NVS(key, nvs_set_u8);
}

bool nvmWriteI16(nvm_size_t key, int16_t value) {
	SET_NVS(key, nvs_set_i16);
}

bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	SET_NVS(key, nvs_set_u16);
}

bool nvmWriteI32(nvm_size_t key, int32_t value) {
	SET_NVS(key, nvs_set_i32);
}

bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	SET_NVS(key, nvs_set_u32);
}

bool nvmWriteI64(nvm_size_t key, int64_t value) {
	SET_NVS(key, nvs_set_i64);
}

bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	SET_NVS(key, nvs_set_u64);
}

bool nvmWriteFloat(nvm_size_t key, float value) {}

bool nvmWriteDouble(nvm_size_t key, double value) {}

bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	GET_NVS(key, nvs_get_u8, (uint8_t*)value);
}

bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_i8, value);
}

bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_u8, value);
}

bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_i16, value);
}

bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_u16, value);
}

bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_i32, value);
}

bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_u32, value);
}

bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_i64, value);
}

bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	GET_NVS(key, nvs_get_u64, value);
}

bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {}

bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {}

#endif