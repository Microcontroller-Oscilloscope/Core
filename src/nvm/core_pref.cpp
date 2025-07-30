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

#ifndef NVM_EEPROM

#define DEFAULT_INT 0
#define DEFAULT_BOOL false
#define DEFAULT_FLOAT NAN

#define GOT_VALUE true
#define WROTE_VALUE false

#define CHAR_KEY_SIZE 3

bool started = false;
uint16_t nvmSize = 0U;
Preferences preferences;

#ifdef __NVM_DEBUG__

template <typename T> void printGotValue(
	enum VarType varType, const uint16_t key, T value, bool gets
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

void nvmWriteFailed(enum VarType varType) {
	printNVM();
	Serial.print(F("pref failed write "));
	printVarType(varType);
	Serial.println(F(""));
	
}
#endif

enum NVMStartCode nvmInit(uint16_t setNVMSize) {
	if (started) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Pref already started"));
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

	nvmSize = setNVMSize;

	#ifdef __NVM_BEGIN__
		//const char *name = "Osc";
		started = preferences.begin("Osc", false);
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

bool nvmStarted(void) {
	if (!started) {
		#ifdef __ERROR_DEBUG__
			printError();
			Serial.print(F("Pref not started"));
		#endif
		return false;
	}
	return true;
}

void keyToChar(uint16_t key, char* keyStr) {
	uint8_t key0 = key & LEAST_BYTE;
	uint8_t key1 = (key & SECOND_LEAST_BYTE) >> 8;

	keyStr[0] = key0;
	keyStr[1] = key1;
	keyStr[2] = '\0';
}

typedef size_t	(Preferences::*PrefPutB)	(const char*, bool);
typedef size_t	(Preferences::*PrefPutI8)	(const char*, int8_t);
typedef size_t	(Preferences::*PrefPutUI8)	(const char*, uint8_t);
typedef size_t	(Preferences::*PrefPutI16)	(const char*, int16_t);
typedef size_t	(Preferences::*PrefPutUI16)	(const char*, uint16_t);
typedef size_t	(Preferences::*PrefPutI32)	(const char*, int32_t);
typedef size_t	(Preferences::*PrefPutUI32)	(const char*, uint32_t);
typedef size_t	(Preferences::*PrefPutI64)	(const char*, int64_t);
typedef size_t	(Preferences::*PrefPutUI64)	(const char*, uint64_t);
typedef size_t	(Preferences::*PrefPutF)	(const char*, float);
typedef size_t	(Preferences::*PrefPutD)	(const char*, double);
typedef size_t	(Preferences::*PrefPutC)	(const char*, const char*);
typedef size_t	(Preferences::*PrefPutS)	(const char*, String);

template <typename PTR, typename VAL>
bool nvmWrite(PTR prefptr, const uint16_t key, VAL value, VarType var) {

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

bool nvmWriteValue(uint16_t key, bool value) {
	return nvmWrite(&Preferences::putBool, key, value, VAR_BOOL);
}

bool nvmWriteValue(uint16_t key, int8_t value) {
	return nvmWrite(&Preferences::putChar, key, value, VAR_INT8);
}

bool nvmWriteValue(uint16_t key, uint8_t value) {
	return nvmWrite(&Preferences::putUChar, key, value, VAR_UINT8);
}

bool nvmWriteValue(uint16_t key, int16_t value) {
	return nvmWrite(&Preferences::putShort, key, value, VAR_INT16);
}

bool nvmWriteValue(uint16_t key, uint16_t value) {
	return nvmWrite(&Preferences::putUShort, key, value, VAR_UINT16);
}

bool nvmWriteValue(uint16_t key, int32_t value) {
	return nvmWrite(&Preferences::putInt, key, value, VAR_INT32);
}

bool nvmWriteValue(uint16_t key, uint32_t value) {
	return nvmWrite(&Preferences::putUInt, key, value, VAR_UINT32);
}

bool nvmWriteValue(uint16_t key, int64_t value) {
	return nvmWrite(&Preferences::putLong64, key, value, VAR_INT64);
}

bool nvmWriteValue(uint16_t key, uint64_t value) {
	return nvmWrite(&Preferences::putULong64, key, value, VAR_UINT64);
}

bool nvmWriteValue(uint16_t key, float value) {
	return nvmWrite(&Preferences::putFloat, key, value, VAR_FLOAT);
}

bool nvmWriteValue(uint16_t key, double value) {
	return nvmWrite(&Preferences::putDouble, key, value, VAR_DOUBLE);
}

bool nvmWriteValue(uint16_t key, char* value, uint8_t maxLength) {

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

typedef bool		(Preferences::*PrefGetB)	(const char*, bool);
typedef int8_t		(Preferences::*PrefGetI8)	(const char*, int8_t);
typedef uint8_t		(Preferences::*PrefGetUI8)	(const char*, uint8_t);
typedef int16_t		(Preferences::*PrefGetI16)	(const char*, int16_t);
typedef uint16_t	(Preferences::*PrefGetUI16)	(const char*, uint16_t);
typedef int32_t		(Preferences::*PrefGetI32)	(const char*, int32_t);
typedef uint32_t	(Preferences::*PrefGetUI32)	(const char*, uint32_t);
typedef int64_t		(Preferences::*PrefGetI64)	(const char*, int64_t);
typedef uint64_t	(Preferences::*PrefGetUI64)	(const char*, uint64_t);
typedef float		(Preferences::*PrefGetF)	(const char*, float);
typedef double		(Preferences::*PrefGetD)	(const char*, double);
typedef size_t		(Preferences::*PrefGetC)	(const char*, char*, size_t);
typedef String		(Preferences::*PrefGetS)	(const char*, String);

template <typename PTR, typename VAL>
bool nvmGet(PTR prefptr, const uint16_t key, VAL *value, VAL defValue, VarType var, bool canDefault) {

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

bool nvmGetValue(uint16_t key, bool *value, bool canDefault) {
	return nvmGet(&Preferences::getBool, key, value, (bool)DEFAULT_BOOL, VAR_BOOL, canDefault);
}

bool nvmGetValue(uint16_t key, int8_t *value, bool canDefault) {
	return nvmGet(&Preferences::getChar, key, value, (int8_t)DEFAULT_INT, VAR_INT8, canDefault);
}

bool nvmGetValue(uint16_t key, uint8_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUChar, key, value, (uint8_t)DEFAULT_INT, VAR_UINT8, canDefault);
}

bool nvmGetValue(uint16_t key, int16_t *value, bool canDefault) {
	return nvmGet(&Preferences::getShort, key, value, (int16_t)DEFAULT_INT, VAR_INT16, canDefault);
}

bool nvmGetValue(uint16_t key, uint16_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUShort, key, value, (uint16_t)DEFAULT_INT, VAR_UINT16, canDefault);
}

bool nvmGetValue(uint16_t key, int32_t *value, bool canDefault) {
	return nvmGet(&Preferences::getInt, key, value, (int32_t)DEFAULT_INT, VAR_INT32, canDefault);
}

bool nvmGetValue(uint16_t key, uint32_t *value, bool canDefault) {
	return nvmGet(&Preferences::getUInt, key, value, (uint32_t)DEFAULT_INT, VAR_UINT32, canDefault);
}

bool nvmGetValue(uint16_t key, int64_t *value, bool canDefault) {
	return nvmGet(&Preferences::getLong64, key, value, (int64_t)DEFAULT_INT, VAR_INT64, canDefault);
}

bool nvmGetValue(uint16_t key, uint64_t *value, bool canDefault) {
	return nvmGet(&Preferences::getULong64, key, value, (uint64_t)DEFAULT_INT, VAR_UINT64, canDefault);
}

bool nvmGetValue(uint16_t key, float *value, bool canDefault) {
	return nvmGet(&Preferences::getFloat, key, value, (float)DEFAULT_FLOAT, VAR_FLOAT, canDefault);
}

bool nvmGetValue(uint16_t key, double *value, bool canDefault) {
	return nvmGet(&Preferences::getDouble, key, value, (double)DEFAULT_FLOAT, VAR_DOUBLE, canDefault);
}

bool nvmGetValue(uint16_t key, char* value, uint8_t maxLength) {

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

#endif