/*
	core_eeprom.cpp - methods for Arduino EEPROM usage
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

#include "core_eeprom.h"

#ifdef NVM_EEPROM

#include <EEPROM.h>
#include "../osc_err/osc_err.h"
#include "../status/debug.h"
#include "../comm/hard_serial/hard_serial.h"

typedef int eeprom_key_t; // type to convert keys to
#define PRINT_KEY hardPrintInt32 // function to print key

bool nvmBegan = false;

memCharString notStartedStr[] = {"EEPROM not started"};
memCharString alreadyStartedStr[] = {"EEPROM already started"};
memCharString defaultSizeStr[] = {"NVM size given was default, not initialized"};
memCharString failStartStr[] = {"'EEPROM' library failed to start"};
memCharString startStr[] = {"Started EEPROM for NVM"};
memCharString couldntWriteStr[] = {"EEPROM couldn't write value '"};
memCharString toKeyStr[] = {"' to key "};
memCharString wroteStr[] = {"EEPROM wrote value '"};
memCharString fromKeyStr[] = {"' from key "};
memCharString nullPtrStr[] = {"Null pointer was given"};
memCharString maxLengthTooShortStr[] = {"Max length not long enough"};
memCharString invalidInputStr[] = {"Invalid input was given"};
memCharString defaultedStr[] = {"NVM defaulted"};
memCharString couldntGetStr[] = {"EEPROM couldn't get value '"};
memCharString gotStr[] = {"EEPROM get value '"};
memCharString maxLength0Str[] = {"Max length 0 not accepted"};
memCharString strTooLongStr[] = {"String too long"};
memCharString failCommitStr[] = {"EEPROM failed commit"};

/**
 * Gets if nvm is started and debugs it
 * 
 * @return if nvm is started
 */
bool nvmStarted() {
	if (!nvmBegan) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(notStartedStr);
		)
		return false;
	}
	return true;
}

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (nvmBegan) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(alreadyStartedStr);
		)
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(defaultSizeStr);
		)
		return NVM_INVALID_SIZE;
	}

	#ifdef __NVM_BEGIN__
		#ifdef __NVM_BEGIN_SIZE__
			#ifdef __NVM_BEGIN_RETURN__
				nvmBegan = EEPROM.begin(setNVMSize);
			#else
				EEPROM.begin(setNVMSize);
				nvmBegan = true;
			#endif
		#else
			#ifdef __NVM_BEGIN_RETURN__
				nvmBegan = EEPROM.begin();
			#else
				EEPROM.begin();
				nvmBegan = true;
			#endif
		#endif
	#else
		nvmBegan = true;
	#endif

	if (!nvmBegan) {
		PRINT_ERROR_DEBUG (
			hardPrintMemCharArrayln(failStartStr);
		)
		return NVM_FAILED;
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArrayln(startStr);
	)

	return NVM_OK;
}

bool nvmMaxSize(nvm_size_t *size) {
	
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

enum NVMDefaultCode nvmSetDefaults(void) {
	// ensures NVM_SIZE isn't too big for microcontroller
	nvm_size_t nvmMaxValue;
	if (nvmMaxSize(&nvmMaxValue)) {
		if (NVM_SIZE > nvmMaxValue) {
			errorLoop(NVM_SIZE_TOO_BIG_FAIL);
			return NVM_DEFAULT_SIZE_TOO_BIG;
		}
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
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
template <typename T>
bool nvmWriteCommit(nvm_size_t key, T value) {
	// inserts value
	EEPROM.put((eeprom_key_t)key, value);
	#ifdef __NVM_COMMIT__
		if (!EEPROM.commit()) {
			PRINT_NVM_DEBUG (
				hardPrintMemCharArrayln(failCommitStr);
				return false;
			)
			return false;
		}
	#endif

	return true;
}

/**
 * Runs whole write process
 * 
 * @param printPtr pointer to variable print function
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
template <typename PRINTPTR, typename T>
bool nvmWrite(PRINTPTR printPtr, nvm_size_t key, T value) {

	if (!nvmStarted()) {
		return false;
	}

	if (!nvmWriteCommit(key, value)) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArray(couldntWriteStr);
			printPtr(value);
			hardPrintMemCharArray(toKeyStr);
			PRINT_KEY(key);
			hardPrintln();
		)
		return false;
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArray(wroteStr);
		printPtr(value);
		hardPrintMemCharArray(fromKeyStr);
		PRINT_KEY(key);
		hardPrintln();
	)
	return true;
}

#ifndef NO_CHAR_ARRAY_SUPPORT

bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmStarted()) {
		return false;
	}

	uint8_t valueLen = charArraySize(value);

	if (valueLen == 0) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(nullPtrStr);
		)
		return false;
	}
	else if (valueLen > maxLength) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(maxLengthTooShortStr);
		)
		return false;
	}
	else if (valueLen == CHAR_LEN_ERROR) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(invalidInputStr);
		)
		return false;
	}

	for (uint8_t i = 0U; i < valueLen; i++) {
		bool result = nvmWrite(&hardPrintChar, key + i, value[i]);

		if (!result) {
			PRINT_NVM_DEBUG (
				hardPrintMemCharArray(couldntWriteStr);
				hardPrintCharArray(value);
				hardPrintMemCharArray(toKeyStr);
				PRINT_KEY(key);
				hardPrintln();
			)
			return false;
		}
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArray(wroteStr);
		hardPrintCharArray(value);
		hardPrintMemCharArray(fromKeyStr);
		PRINT_KEY(key);
		hardPrintln();
	)

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
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(defaultedStr);
		)
		return false;
	}
	return true;
}

/**
 * Runs whole get process
 * 
 * @param printPtr pointer to variable print function
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param defaultValue default value from get
 * @param canDefault if recieving value can default
 * 
 * @return if get was successful
 */
template <typename PRINTPTR, typename T>
bool nvmGet(PRINTPTR printPtr, nvm_size_t key, T *value, T defaultValue, bool canDefault) {
	if (!nvmStarted()) {
		return false;
	}

	if (!nvmGetVal(key, value, defaultValue, canDefault)) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArray(couldntGetStr);
			printPtr(*value);
			hardPrintMemCharArray(fromKeyStr);
			PRINT_KEY(key);
			hardPrintln();
		)
		return false;
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArray(gotStr);
		printPtr(*value);
		hardPrintMemCharArray(fromKeyStr);
		PRINT_KEY(key);
		hardPrintln();
	)
	return true;
}

#ifndef NO_CHAR_ARRAY_SUPPORT

bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmStarted()) {
		return false;
	}

	if (!validCharPointer(value)) {
		return false;
	}

	if (maxLength == 0U) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(maxLength0Str);
		)
		return false;
	}

	for (uint8_t i = 0U; i < maxLength; i++) {
		char letter;
		if (!nvmGet(&hardPrintChar, key+i, &letter, (char)DEFAULT_INT, CAN_DEFAULT)) {
			return false;
		}
		value[i] = letter;
		if (letter == END_OF_CHAR) {
			PRINT_NVM_DEBUG (
				hardPrintMemCharArray(gotStr);
				hardPrintCharArray(value);
				hardPrintMemCharArray(fromKeyStr);
				PRINT_KEY(key);
				hardPrintln();
			)
			return true;
		}
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArrayln(strTooLongStr);
	)
	return false;
}

#endif

bool nvmWriteBool(nvm_size_t key, bool value) {
	return nvmWrite(&hardPrintBool, key, value);
}

bool nvmWriteI8(nvm_size_t key, int8_t value) {
	return nvmWrite(&hardPrintInt8, key, value);
}

bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	return nvmWrite(&hardPrintUInt8, key, value);
}

bool nvmWriteI16(nvm_size_t key, int16_t value) {
	return nvmWrite(&hardPrintInt16, key, value);
}

bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	return nvmWrite(&hardPrintUInt16, key, value);
}

bool nvmWriteI32(nvm_size_t key, int32_t value) {
	return nvmWrite(&hardPrintInt32, key, value);
}

bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	return nvmWrite(&hardPrintUInt32, key, value);
}

bool nvmWriteI64(nvm_size_t key, int64_t value) {
	return nvmWrite(&hardPrintInt64, key, value);
}

bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	return nvmWrite(&hardPrintUInt64, key, value);
}

bool nvmWriteFloat(nvm_size_t key, float value) {
	return nvmWrite(&hardPrintFloat, key, value);
}

bool nvmWriteDouble(nvm_size_t key, double value) {
	return nvmWrite(&hardPrintDouble, key, value);
}

bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	return nvmGet(&hardPrintBool, key, value, (bool)DEFAULT_BOOL, canDefault);
}

bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	return nvmGet(&hardPrintInt8, key, value, (int8_t)DEFAULT_INT, canDefault);
}

bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	return nvmGet(&hardPrintUInt8, key, value, (uint8_t)DEFAULT_INT, canDefault);
}

bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	return nvmGet(&hardPrintInt16, key, value, (int16_t)DEFAULT_INT, canDefault);
}

bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	return nvmGet(&hardPrintUInt16, key, value, (uint16_t)DEFAULT_INT, canDefault);
}

bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	return nvmGet(&hardPrintInt32, key, value, (int32_t)DEFAULT_INT, canDefault);
}

bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	return nvmGet(&hardPrintUInt32, key, value, (uint32_t)DEFAULT_INT, canDefault);
}

bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	return nvmGet(&hardPrintInt64, key, value, (int64_t)DEFAULT_INT, canDefault);
}

bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	return nvmGet(&hardPrintUInt64, key, value, (uint64_t)DEFAULT_INT, canDefault);
}

bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {
	return nvmGet(&hardPrintFloat, key, value, (float)DEFAULT_FLOAT, canDefault);
}

bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {
	return nvmGet(&hardPrintDouble, key, value, (double)DEFAULT_FLOAT, canDefault);
}

#endif