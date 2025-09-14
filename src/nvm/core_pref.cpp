/*
	core_pref.h - methods for Preferences usage
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

#define GOT_VALUE true
#define WROTE_VALUE false

#define CHAR_KEY_SIZE NVM_MAX_SIZE_BYTES + 1U

bool started = false;
Preferences preferences;

#ifdef __NVM_DEBUG__

/**
 * Prints value retrieved from operations
 * 
 * @param varType variable type to print
 * @param key key linked to value
 * @param value value retrieved
 * @param gets if getting or setting
 */
template <typename T> void printGotValue(
	enum VarType varType, const nvm_size_t key, T value, bool gets
) {
	printNVM();
	Serial.print(F("Pref "));

	if (gets) {
		Serial.print(F("got, "));
	}
	else {
		Serial.print(F("wrote, "));
	}

	printVarType(varType);
	Serial.print(F(": '"));
	Serial.print(value);
	Serial.print(F("', key: '"));
	Serial.print(key);
	Serial.println(F("'"));
}

/**
 * Prints that pref failed write
 * 
 * @param varType variable type attempted
 */
void nvmWriteFailed(enum VarType varType) {
	printNVM();
	Serial.print(F("pref failed write "));
	printVarType(varType);
	Serial.println(F(""));
}

#endif

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (started) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Pref already started"));
		#endif
		return NVM_STARTED;
	}

	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("NVM size given was default, not initialized"));
		#endif
		return NVM_INVALID_SIZE;
	}

	#ifdef __NVM_BEGIN__
		#ifdef __NVM_BEGIN_RETURN__
			started = preferences.begin("Osc", false);
		#else
			preferences.begin("Osc", false);
			started = true;
		#endif
	#else
		started = true;
	#endif

	if (!started) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.println(F("Preferences lib failed to start"));
		#endif
		return NVM_FAILED;
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.println(F("Started Preferences for NVM"));
	#endif

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
	if (!started) {
		return false;
	}

	preferences.end();
	started = false;

	return true;
}

bool nvmMaxSize(nvm_size_t *size) {

	if (started) {
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
			#ifdef __NVM_DEBUG__
				printNVM();
				Serial.println(F("Pref failed to erease nvs"));
			#endif
			return false;
		}

		result = nvs_flash_init();
		if (result) {
			#ifdef __NVM_DEBUG__
				printNVM();
				Serial.println(F("Pref failed to init nvs"));
			#endif
			return false;
		}
		return true;
	#else
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.println(F("No Pref clear method implemented"));
		#endif
		return false;
	#endif
}

NVMDefaultCode nvmSetDefaults(void) {

	// ensures NVM_SIZE isn't too big for microcontroller
	nvm_size_t nvmMaxValue;
	if (nvmMaxSize(&nvmMaxValue)) {
		if (NVM_SIZE > nvmMaxValue) {
			#ifndef __TEST_CASES__
				errorLoop(NVM_SIZE_TOO_BIG_FAIL);
			#endif
			return NVM_DEFAULT_SIZE_TOO_BIG;
		}
	}
	else {
		// if nvm not started or unable to get size
		return NVM_DEFAULT_FAIL_MAX_SIZE;
	}

	// ensures clear works
	if (!nvmClear()) {
		printDefaultDebug(F("nvm default failed clear"));
		return NVM_DEFAULT_FAIL_CLEAR;
	}

	// stops nvm
	if (!nvmStop()) {
		printDefaultDebug(F("nvm default failed stop"));
		return NVM_DEFAULT_FAIL_STOP;
	}

	// restarts nvm for operations
	NVMStartCode startCode = nvmInit(NVM_SIZE);
	if (startCode != NVM_OK) {
		printDefaultDebug(F("nvm default failed init"));
		return NVM_DEFAULT_FAIL_INIT;
	}

	// writes critical values
	NVMDefaultCode code = nvmSetCritDefaults(nvmMaxValue);
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
	if (!started) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.println(F("Pref not started"));
		#endif
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
			printGotValue(var, key, value, WROTE_VALUE);
		}
	#endif

	return (bool)result;
}

bool nvmWriteValue(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmStarted()) {
		return false;
	}

	uint8_t valueLen = charArraySize(value);

	if (valueLen == 0) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Null pointer was given"));
		#endif
		return false;
	}
	else if (valueLen > maxLength) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Max length not long enough"));
		#endif
		return false;
	}
	else if (valueLen == CHAR_LEN_ERROR) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Invalid input was given"));
		#endif
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
		printGotValue(VAR_CHAR_ARRAY, key, value, WROTE_VALUE);
	}
	#endif

	return (bool)result;
}

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

	if (!nvmStarted()) {
		return false;
	}

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	*value = (preferences.*prefptr)(keyStr, defValue);

	if (!canDefault && *value == defValue) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Can't get default value"));
		#endif
		return false;
	}

	#ifdef __NVM_DEBUG__
		printGotValue(var, key, *value, GOT_VALUE);
	#endif

	return true;
}

bool nvmGetValue(nvm_size_t key, char* value, uint8_t maxLength) {

	if (!nvmStarted()) {
		return false;
	}

	if (!validCharPointer(value)) {
		return false;
	}

	if (maxLength == 0U) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Max length 0 not accepted"));
		#endif
		return false;
	}

	char keyStr[CHAR_KEY_SIZE];
	keyToChar(key, keyStr);

	size_t charSize = preferences.getString(keyStr, value, maxLength);
	uint8_t valueLen = charArraySize(value);

	if (valueLen != charSize) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Error getting value"));
		#endif
		return false;
	}

	#ifdef __NVM_DEBUG__
		printGotValue(VAR_CHAR_ARRAY, key, value, GOT_VALUE);
	#endif

	return true;
}

bool nvmWriteValue(nvm_size_t key, bool value) {
	return nvmWrite(&Preferences::putBool, key, value, VAR_BOOL);
}

bool nvmWriteValue(nvm_size_t key, int8_t value) {
	return nvmWrite(&Preferences::putChar, key, value, VAR_INT8);
}

bool nvmWriteValue(nvm_size_t key, uint8_t value) {
	return nvmWrite(&Preferences::putUChar, key, value, VAR_UINT8);
}

bool nvmWriteValue(nvm_size_t key, int16_t value) {
	return nvmWrite(&Preferences::putShort, key, value, VAR_INT16);
}

bool nvmWriteValue(nvm_size_t key, uint16_t value) {
	return nvmWrite(&Preferences::putUShort, key, value, VAR_UINT16);
}

bool nvmWriteValue(nvm_size_t key, int32_t value) {
	return nvmWrite(&Preferences::putInt, key, value, VAR_INT32);
}

bool nvmWriteValue(nvm_size_t key, uint32_t value) {
	return nvmWrite(&Preferences::putUInt, key, value, VAR_UINT32);
}

bool nvmWriteValue(nvm_size_t key, int64_t value) {
	return nvmWrite(&Preferences::putLong64, key, value, VAR_INT64);
}

bool nvmWriteValue(nvm_size_t key, uint64_t value) {
	return nvmWrite(&Preferences::putULong64, key, value, VAR_UINT64);
}

bool nvmWriteValue(nvm_size_t key, float value) {
	return nvmWrite(&Preferences::putFloat, key, value, VAR_FLOAT);
}

bool nvmWriteValue(nvm_size_t key, double value) {
	return nvmWrite(&Preferences::putDouble, key, value, VAR_DOUBLE);
}

bool nvmGetValue(nvm_size_t key, bool *value, bool canDefault) {
	return nvmGet(&Preferences::getBool, key, value, (bool)DEFAULT_BOOL, VAR_BOOL, canDefault);
}

bool nvmGetValue(nvm_size_t key, int8_t *value, bool canDefault) {
	return nvmGet(&Preferences::getChar, key, value, (int8_t)DEFAULT_INT, VAR_INT8, canDefault);
}

bool nvmGetValue(nvm_size_t key, uint8_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUChar, key, value, (uint8_t)DEFAULT_INT, VAR_UINT8, canDefault);
}

bool nvmGetValue(nvm_size_t key, int16_t *value, bool canDefault) {
	return nvmGet(&Preferences::getShort, key, value, (int16_t)DEFAULT_INT, VAR_INT16, canDefault);
}

bool nvmGetValue(nvm_size_t key, uint16_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUShort, key, value, (uint16_t)DEFAULT_INT, VAR_UINT16, canDefault);
}

bool nvmGetValue(nvm_size_t key, int32_t *value, bool canDefault) {
	return nvmGet(&Preferences::getInt, key, value, (int32_t)DEFAULT_INT, VAR_INT32, canDefault);
}

bool nvmGetValue(nvm_size_t key, uint32_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUInt, key, value, (uint32_t)DEFAULT_INT, VAR_UINT32, canDefault);
}

bool nvmGetValue(nvm_size_t key, int64_t *value, bool canDefault) {
	return nvmGet(&Preferences::getLong64, key, value, (int64_t)DEFAULT_INT, VAR_INT64, canDefault);
}

bool nvmGetValue(nvm_size_t key, uint64_t *value, bool canDefault) {
	return nvmGet(&Preferences::getULong64, key, value, (uint64_t)DEFAULT_INT, VAR_UINT64, canDefault);
}

bool nvmGetValue(nvm_size_t key, float *value, bool canDefault) {
	return nvmGet(&Preferences::getFloat, key, value, (float)DEFAULT_FLOAT, VAR_FLOAT, canDefault);
}

bool nvmGetValue(nvm_size_t key, double *value, bool canDefault) {
	return nvmGet(&Preferences::getDouble, key, value, (double)DEFAULT_FLOAT, VAR_DOUBLE, canDefault);
}

#endif