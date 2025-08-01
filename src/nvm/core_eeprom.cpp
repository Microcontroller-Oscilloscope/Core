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

#ifndef NVM_PREF

#include <EEPROM.h>

bool started = false;
uint16_t nvmSize = 0U;

/**
 * Gets if nvm is started and debugs it
 * 
 * @return if nvm is started
 */
bool nvmStarted() {
	if (!started) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("EEPROM not started"));
		#endif
		return false;
	}
	return true;
}

enum NVMStartCode nvmInit(uint16_t setNVMSize) {
	if (started) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("EEPROM already started"));
		#endif
		return NVM_STARTED;
	}

	if (setNVMSize == (uint16_t)DEFAULT_NVM_SIZE) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("NVM size given was default, not initialized"));
		#endif
		return NVM_INVALID_SIZE;
	}

	nvmSize = nvmSize;

	#ifdef __NVM_BEGIN__
		#ifdef __NVM_BEGIN_SIZE__
			#ifdef __NVM_BEGIN_RETURN__
				started = EEPROM.begin(nvmSize);
			#else
				EEPROM.begin(nvmSize);
				started = true;
			#endif
		#else
			#ifdef __NVM_BEGIN_RETURN__
				started = EEPROM.begin();
			#else
				EEPROM.begin();
				started = true;
			#endif
		#endif
	#else
		started = true;
	#endif

	if (!started) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.println(F("'EEPROM' library failed to start"));
		#endif
		return NVM_FAILED;
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.println(F("Started EEPROM for NVM"));
	#endif

	return NVM_OK;
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
bool nvmWriteCommit(uint16_t key, T value) {
	// inserts value
	EEPROM.put((int)key, value);
	#ifdef __NVM_COMMIT__
		EEPROM.commit();
	#endif

	return true;
}

/**
 * Runs whole write process
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param var variable type for printing
 * 
 * @return if write was valid
 */
template <typename T>
bool nvmWrite(uint16_t key, T value) {

	if (!nvmStarted()) {
		return false;
	}

	bool result = nvmWriteCommit(key, value);

	if (!result) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.print(F("EEPROM couldn't write value '"));
			Serial.print(value);
			Serial.print(F("' to key "));
			Serial.println(key);
		#endif
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.print(F("EEPROM wrote value '"));
		Serial.print(value);
		Serial.print(F("' from key "));
		Serial.println(key);
	#endif

	return result;
}

#ifndef INT64_SUPPORT

/**
 * Runs whole write process for i64 and u64
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param var variable type for printing
 * 
 * @return if write was valid
 */
template <typename T>
bool nvmWrite64(uint16_t key, T value) {
	if (!nvmStarted()) {
		return false;
	}

	bool result = nvmWriteCommit(key, value);

	if (!result) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.print(F("EEPROM couldn't write value '"));
			printInt64(value);
			Serial.print(F("' to key "));
			Serial.println(key);
		#endif
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.print(F("EEPROM wrote value '"));
		printInt64(value);
		Serial.print(F("' from key "));
		Serial.println(key);
	#endif

	return result;
}

#endif

bool nvmWriteValue(uint16_t key, bool value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, int8_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, uint8_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, int16_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, uint16_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, int32_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, uint32_t value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, int64_t value) {
	#ifdef INT64_SUPPORT
	return nvmWrite(key, value);
	#else
	return nvmWrite64(key, value);
	#endif
}

bool nvmWriteValue(uint16_t key, uint64_t value) {
	#ifdef INT64_SUPPORT
	return nvmWrite(key, value);
	#else
	return nvmWrite64(key, value);
	#endif
}

bool nvmWriteValue(uint16_t key, float value) {
	return nvmWrite(key, value);
}

bool nvmWriteValue(uint16_t key, double value) {
	return nvmWrite(key, value);
}

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if get was successful
 */
template <typename T>
bool nvmGetVal(uint16_t key, T *value) {
	EEPROM.get((int)key, *value);
	return true;
}

/**
 * Runs whole get process
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if get was successful
 */
template <typename T>
bool nvmGet(uint16_t key, T *value) {
	if (!nvmStarted()) {
		return false;
	}

	bool result = nvmGetVal(key, value);

	if (!result) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.print(F("EEPROM couldn't get value '"));
			Serial.print(*value);
			Serial.print(F("' to key "));
			Serial.println(key);
		#endif
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.print(F("EEPROM get value '"));
		Serial.print(*value);
		Serial.print(F("' from key "));
		Serial.println(key);
	#endif

	return result;
}

#ifndef INT64_SUPPORT

/**
 * Runs whole get process for i64 and u64
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if get was successful
 */
template <typename T>
bool nvmGet64(uint16_t key, T *value) {
	if (!nvmStarted()) {
		return false;
	}

	bool result = nvmGetVal(key, value);

	if (!result) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.print(F("EEPROM couldn't get value '"));
			printInt64(*value);
			Serial.print(F("' to key "));
			Serial.println(key);
		#endif
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.print(F("EEPROM get value '"));
		printInt64(*value);
		Serial.print(F("' from key "));
		Serial.println(key);
	#endif

	return result;
}

#endif

bool nvmGetValue(uint16_t key, bool *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, int8_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, uint8_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, int16_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, uint16_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, int32_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, uint32_t *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, int64_t *value) {
	#ifdef INT64_SUPPORT
	return nvmGet(key, value);
	#else
	return nvmGet64(key, value);
	#endif
}

bool nvmGetValue(uint16_t key, uint64_t *value) {
	#ifdef INT64_SUPPORT
	return nvmGet(key, value);
	#else
	return nvmGet64(key, value);
	#endif
}

bool nvmGetValue(uint16_t key, float *value) {
	return nvmGet(key, value);
}

bool nvmGetValue(uint16_t key, double *value) {
	return nvmGet(key, value);
}

#endif