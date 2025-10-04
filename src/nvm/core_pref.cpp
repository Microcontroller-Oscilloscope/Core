/*
	core_pref.cpp - methods for Preferences usage
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

#include "core_pref.h"

#ifdef NVM_PREF

#include <Preferences.h>
#include "../osc_err/osc_err.h"
#include "../status/debug.h"
#include "../comm/hard_serial/hard_serial.h"

#define GOT_VALUE true
#define WROTE_VALUE false

#define CHAR_KEY_SIZE NVM_MAX_SIZE_BYTES + 1U
#define OSC_NAME_SPACE "Osc"
#define PRINT_KEY hardPrintUInt16 // function to print key

bool nvmBegan = false;
Preferences preferences;

#define NVM_DEBUG defined(__NVM_DEBUG__) || defined(__ERROR_DEBUG__) // stores if debugging is enabled

#if NVM_DEBUG

	memCharString prefStr[] = {"Pref "};
	memCharString gotStr[] = {"got, "};
	memCharString wroteStr[] = {"wrote, "};
	memCharString colonStr[] = {": '"};
	memCharString keyStr[] = {"', key: "};
	memCharString failWriteStr[] = {"pref failed write "};
	memCharString nvmStartedStr[] = {"Pref already started"};
	memCharString nvmBadSizeStr[] = {"NVM size given was default, not initialized"};
	memCharString nvmFailStartStr[] = {"Preferences lib failed to start"};
	memCharString nvmStartStr[] = {"Started Preferences for NVM"};
	memCharString nvmEreaseFailStr[] = {"Pref failed to erease nvs"};
	memCharString nvmInitFailStr[] = {"Pref failed to init nvs"};
	memCharString nvmNoClearMethodStr[] = {"No Pref clear method implemented"};
	memCharString nvmNotStartedStr[] = {"Pref not started"};
	memCharString nullPointerStr[] = {"Null pointer was given"};
	memCharString maxLengthTooShortStr[] = {"Max length not long enough"};
	memCharString invalidInputStr[] = {"Invalid input was given"};
	memCharString noDefaultStr[] = {"Can't get default value"};
	memCharString maxLength0Str[] = {"Max length 0 not accepted"};
	memCharString errorGetValueStr[] = {"Error getting value"};

#endif

#ifdef __NVM_DEBUG__

	/**
	 * Prints value retrieved from operations
	 * 
	 * @param printPtr pointer to variable print function
	 * @param key key linked to value
	 * @param value value retrieved
	 * @param gets if getting or setting
	 */
	template <typename PRINTPTR, typename T> void printGotValue(
		PRINTPTR printPtr, const nvm_size_t key, T value, bool gets
	) {
		printNVM();
		hardPrintMemCharArray(prefStr);

		if (gets) {
			hardPrintMemCharArray(gotStr);
		}
		else {
			hardPrintMemCharArray(wroteStr);
		}

		hardPrintMemCharArray(colonStr);
		printPtr(value);
		hardPrintMemCharArray(keyStr);
		PRINT_KEY(key);
		hardPrintln();
	}

	/**
	 * Prints that pref failed write
	 * 
	 * @param varType variable type attempted
	 */
	void nvmWriteFailed(enum VarType varType) {
		printNVM();
		hardPrintMemCharArray(failWriteStr);
		printVarType(varType);
		hardPrintln();
	}

#endif

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (nvmBegan) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(nvmStartedStr);
		)
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(nvmBadSizeStr);
		)
		return NVM_INVALID_SIZE;
	}

	#ifdef __NVM_BEGIN__
		#ifdef __NVM_BEGIN_RETURN__
			nvmBegan = preferences.begin(OSC_NAME_SPACE, false);
		#else
			preferences.begin(OSC_NAME_SPACE, false);
			nvmBegan = true;
		#endif
	#else
		nvmBegan = true;
	#endif

	if (!nvmBegan) {
		PRINT_ERROR_DEBUG (
			hardPrintMemCharArrayln(nvmFailStartStr);
		)
		return NVM_FAILED;
	}

	PRINT_NVM_DEBUG (
		hardPrintMemCharArrayln(nvmStartStr);
	)

	return NVM_OK;
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

	preferences.end();
	nvmBegan = false;

	return true;
}

bool nvmMaxSize(nvm_size_t *size) {

	if (nvmBegan) {
		*size = NVM_MAX_SIZE;
		return true;
	}

	*size = DEFAULT_NVM_SIZE;
	return false;
}

#ifdef ESP32DEVC
	#include "nvs.h"
	#include "nvs_flash.h"
#endif

/**
 * Clears all nvm data
 * 
 * @warning WILL CLEAR NVM DATA
 * @warning NOT INTENDED FOR EXTERNAL USE
 * 
 * @return if clear was successful
 */
bool nvmClear(void) {

	#ifdef ESP32DEVC
		esp_err_t result = nvs_flash_erase();
		if (result) {
			PRINT_NVM_DEBUG (
				hardPrintMemCharArrayln(nvmEreaseFailStr);
			)
			return false;
		}

		result = nvs_flash_init();
		if (result) {
			PRINT_NVM_DEBUG (
				hardPrintMemCharArrayln(nvmInitFailStr);
			)
			return false;
		}
		return true;
	#else
		PRINT_ERROR_DEBUG (
			hardPrintMemCharArrayln(nvmNoClearMethodStr);
		)
		return false;
	#endif
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
 * Prints if Pref wasn't started
 * 
 * @return if Pref was started
 */
bool nvmStarted(void) {
	if (!nvmBegan) {
		PRINT_ERROR_DEBUG (
			hardPrintMemCharArray(nvmNotStartedStr);
		)
		return false;
	}
	return true;
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

#if NVM_DEBUG

/**
 * Writes value to Preferences
 * 
 * @param printPtr pointer to variable print function
 * @param prefptr pointer to Preferences put function
 * @param key key of value to write
 * @param value value to write to key
 * @param var variable type for debug printing
 * 
 * @return if write was successful
 */
template <typename PRINTPTR, typename PTR, typename VAL>
bool nvmWrite(PRINTPTR printPtr, PTR prefptr, const nvm_size_t key, VAL value, VarType var) {

#else

/**
 * Writes value to Preferences
 * 
 * @param prefptr pointer to Preferences put function
 * @param key key of value to write
 * @param value value to write to key
 * @param var variable type for debug printing
 * 
 * @return if write was successful
 */
template <typename PTR, typename VAL>
bool nvmWrite(PTR prefptr, const nvm_size_t key, VAL value, VarType var) {

#endif

	if (!nvmStarted()) {
		return false;
	}

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	size_t result = (preferences.*prefptr)(keyStr, value);

	#ifdef __NVM_DEBUG__
		if (!result) {
			nvmWriteFailed(var);
		}
		else {
			printGotValue(printPtr, key, value, WROTE_VALUE);
		}
	#endif

	return (bool)result;
}

bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmStarted()) {
		return false;
	}

	uint8_t valueLen = charArraySize(value);

	if (valueLen == 0) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(nullPointerStr);
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

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	size_t result = preferences.putString(keyStr, value) + 1U;

	#ifdef __NVM_DEBUG__
	if ((uint8_t)result != valueLen) {
		nvmWriteFailed(VAR_CHAR_ARRAY);
	}
	else {
		printGotValue(&hardPrintCharArray, key, value, WROTE_VALUE);
	}
	#endif

	return (bool)result;
}

#if NVM_DEBUG

/**
 * Gets value from Preferences
 * 
 * @param printPtr pointer to variable print function
 * @param prefptr pointer to Preferences get function
 * @param key key of value to read
 * @param value value to read from key
 * @param defValue default value of get function
 * @param var variable type for debug printing
 * @param canDefault if return value can be default value
 * 
 * @return if get was successful
 */
template <typename PRINTPTR, typename PTR, typename VAL>
bool nvmGet(PRINTPTR printPtr, PTR prefptr, const nvm_size_t key, VAL *value, VAL defValue, VarType var, bool canDefault) {

#else

/**
 * Gets value from Preferences
 * 
 * @param prefptr pointer to Preferences get function
 * @param key key of value to read
 * @param value value to read from key
 * @param defValue default value of get function
 * @param var variable type for debug printing
 * @param canDefault if return value can be default value
 * 
 * @return if get was successful
 */
template <typename PTR, typename VAL>
bool nvmGet(PTR prefptr, const nvm_size_t key, VAL *value, VAL defValue, VarType var, bool canDefault) {

#endif

	if (!nvmStarted()) {
		return false;
	}

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	*value = (preferences.*prefptr)(keyStr, defValue);

	if (!canDefault && *value == defValue) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(noDefaultStr);
		)
		return false;
	}

	#ifdef __NVM_DEBUG__
		printGotValue(printPtr, key, *value, GOT_VALUE);
	#endif

	return true;
}

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

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	size_t charSize = preferences.getString(keyStr, value, maxLength);
	uint8_t valueLen = charArraySize(value);

	if (valueLen != charSize) {
		PRINT_NVM_DEBUG (
			hardPrintMemCharArrayln(errorGetValueStr);
		)
		return false;
	}

	#ifdef __NVM_DEBUG__
		printGotValue(&hardPrintCharArray, key, value, GOT_VALUE);
	#endif

	return true;
}

bool nvmWriteBool(nvm_size_t key, bool value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintBool, 
		#endif
	&Preferences::putBool, key, value, VAR_BOOL);
}

bool nvmWriteI8(nvm_size_t key, int8_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintInt8, 
		#endif
	&Preferences::putChar, key, value, VAR_INT8);
}

bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintUInt8, 
		#endif
	&Preferences::putUChar, key, value, VAR_UINT8);
}

bool nvmWriteI16(nvm_size_t key, int16_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintInt16, 
		#endif
	&Preferences::putShort, key, value, VAR_INT16);
}

bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintUInt16, 
		#endif
	&Preferences::putUShort, key, value, VAR_UINT16);
}

bool nvmWriteI32(nvm_size_t key, int32_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintInt32, 
		#endif
	&Preferences::putInt, key, value, VAR_INT32);
}

bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintUInt32, 
		#endif
	&Preferences::putUInt, key, value, VAR_UINT32);
}

bool nvmWriteI64(nvm_size_t key, int64_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintInt64, 
		#endif
	&Preferences::putLong64, key, value, VAR_INT64);
}

bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintUInt64, 
		#endif
	&Preferences::putULong64, key, value, VAR_UINT64);
}

bool nvmWriteFloat(nvm_size_t key, float value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintFloat, 
		#endif
	&Preferences::putFloat, key, value, VAR_FLOAT);
}

bool nvmWriteDouble(nvm_size_t key, double value) {
	return nvmWrite(
		#if NVM_DEBUG
			&hardPrintDouble, 
		#endif
	&Preferences::putDouble, key, value, VAR_DOUBLE);
}

bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintBool, 
		#endif
	&Preferences::getBool, key, value, (bool)DEFAULT_BOOL, VAR_BOOL, canDefault);
}

bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintInt8, 
		#endif
	&Preferences::getChar, key, value, (int8_t)DEFAULT_INT, VAR_INT8, canDefault);
}

bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintUInt8, 
		#endif
	&Preferences::getUChar, key, value, (uint8_t)DEFAULT_INT, VAR_UINT8, canDefault);
}

bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintInt16, 
		#endif
	&Preferences::getShort, key, value, (int16_t)DEFAULT_INT, VAR_INT16, canDefault);
}

bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintUInt16, 
		#endif
	&Preferences::getUShort, key, value, (uint16_t)DEFAULT_INT, VAR_UINT16, canDefault);
}

bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintInt32, 
		#endif
	&Preferences::getInt, key, value, (int32_t)DEFAULT_INT, VAR_INT32, canDefault);
}

bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintUInt32, 
		#endif
	&Preferences::getUInt, key, value, (uint32_t)DEFAULT_INT, VAR_UINT32, canDefault);
}

bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintInt64, 
		#endif
	&Preferences::getLong64, key, value, (int64_t)DEFAULT_INT, VAR_INT64, canDefault);
}

bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintUInt64, 
		#endif
	&Preferences::getULong64, key, value, (uint64_t)DEFAULT_INT, VAR_UINT64, canDefault);
}

bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintFloat, 
		#endif
	&Preferences::getFloat, key, value, (float)DEFAULT_FLOAT, VAR_FLOAT, canDefault);
}

bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {
	return nvmGet(
		#if NVM_DEBUG
			&hardPrintDouble, 
		#endif
	&Preferences::getDouble, key, value, (double)DEFAULT_FLOAT, VAR_DOUBLE, canDefault);
}

#endif