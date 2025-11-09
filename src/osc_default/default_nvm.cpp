/*
	default_nvm.cpp - methods for Arduino EEPROM usage
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

#include "../compile_flags/compile_flags.h"

#if ENVIRONMENT_ARDUINO

#include <EEPROM.h>

#include "../nvm/nvm.h"
#include "../osc_err/osc_err.h"
#include "../status/status.h"
#include "../comm/hard_serial/hard_serial.h"

typedef int eeprom_key_t; // type to convert keys to

bool nvmBegan = false;

extern "C" enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (nvmBegan) {
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		return NVM_INVALID_SIZE;
	}

	#ifdef __NVM_BEGIN__
		#ifdef __NVM_BEGIN_RETURN__
			nvmBegan = 
		#endif
		EEPROM.begin(
			#ifdef __NVM_BEGIN_SIZE__
				setNVMSize
			#endif
		);
	#endif

	#if !defined(__NVM_BEGIN__) || (defined(__NVM_BEGIN__) && !defined(__NVM_BEGIN_RETURN__))
		nvmBegan = true;
	#endif

	if (!nvmBegan) {
		return NVM_FAILED;
	}

	return NVM_OK;
}

extern "C" bool nvmMaxSize(nvm_size_t *size) {
	
	if (nvmBegan) {
		*size = (nvm_size_t)EEPROM.length();
		if (*size == 0) {
			*size = NVM_MAX_SIZE;
		}
		return true;
	}

	*size = DEFAULT_NVM_SIZE;
	return false;
}

extern "C" enum NVMDefaultCode nvmSetDefaults(void) {
	// ensures NVM_SIZE isn't too big for microcontroller
	nvm_size_t nvmMaxValue;
	if (nvmMaxSize(&nvmMaxValue)) {
		#ifdef NVM_SIZE
			if (NVM_SIZE > nvmMaxValue) {
				errorLoop(NVM_SIZE_TOO_BIG_FAIL);
				return NVM_DEFAULT_SIZE_TOO_BIG;
			}
		#endif
	}
	else {
		// if nvm not started or unable to get size
		return NVM_DEFAULT_FAIL_MAX_SIZE;
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
 * Runs whole write process
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
template <typename T>
bool nvmWrite(nvm_size_t key, T value) {

	if (!nvmBegan) {
		return false;
	}

	// inserts value
	EEPROM.put((eeprom_key_t)key, value);
	#ifdef __NVM_COMMIT__
		if (!EEPROM.commit()) {
			return false;
		}
	#endif

	return true;
}

#ifndef NO_CHAR_ARRAY_SUPPORT

extern "C" bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmBegan) {
		return false;
	}

	uint8_t valueLen = charArraySize(value);

	if (valueLen == 0) {
		return false;
	}
	else if (valueLen > maxLength) {
		return false;
	}
	else if (valueLen == CHAR_LEN_ERROR) {
		return false;
	}

	for (uint8_t i = 0U; i < valueLen; i++) {
		bool result = nvmWrite(key + i, value[i]);

		if (!result) {
			return false;
		}
	}

	return true;
}

#endif

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param defaultValue default value from get
 * @param canDefault if recieving value can default
 * 
 * @return if get was successful
 */
template <typename T>
bool nvmGetVal(nvm_size_t key, T *value, T defaultValue, bool canDefault) {
	EEPROM.get((eeprom_key_t)key, *value);

	if (*value == defaultValue && !canDefault) {
		return false;
	}
	return true;
}

/**
 * Runs whole get process
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param defaultValue default value from get
 * @param canDefault if recieving value can default
 * 
 * @return if get was successful
 */
template <typename T>
bool nvmGet(nvm_size_t key, T *value, T defaultValue, bool canDefault) {

	if (!nvmBegan) {
		return false;
	}
	if (!nvmGetVal(key, value, defaultValue, canDefault)) {
		return false;
	}

	return true;
}

#ifndef NO_CHAR_ARRAY_SUPPORT

extern "C" bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmBegan) {
		return false;
	}

	if (!validCharPointer(value)) {
		return false;
	}

	if (maxLength == 0U) {
		return false;
	}

	for (uint8_t i = 0U; i < maxLength; i++) {
		char letter;
		if (!nvmGet(key+i, &letter, (char)DEFAULT_INT, CAN_DEFAULT)) {
			return false;
		}
		value[i] = letter;
		if (letter == END_OF_CHAR) {
			return true;
		}
	}

	return false;
}

#endif

extern "C" bool nvmWriteBool(nvm_size_t key, bool value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteI8(nvm_size_t key, int8_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteI16(nvm_size_t key, int16_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteI32(nvm_size_t key, int32_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteI64(nvm_size_t key, int64_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteFloat(nvm_size_t key, float value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmWriteDouble(nvm_size_t key, double value) {
	return nvmWrite(key, value);
}

extern "C" bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	return nvmGet(key, value, (bool)DEFAULT_BOOL, canDefault);
}

extern "C" bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	return nvmGet(key, value, (int8_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	return nvmGet(key, value, (uint8_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	return nvmGet(key, value, (int16_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	return nvmGet(key, value, (uint16_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	return nvmGet(key, value, (int32_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	return nvmGet(key, value, (uint32_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	return nvmGet(key, value, (int64_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	return nvmGet(key, value, (uint64_t)DEFAULT_INT, canDefault);
}

extern "C" bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {
	return nvmGet(key, value, (float)DEFAULT_FLOAT, canDefault);
}

extern "C" bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {
	return nvmGet(key, value, (double)DEFAULT_FLOAT, canDefault);
}

#endif