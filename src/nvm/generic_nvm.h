/*
	generic_nvm.h - methods for generic nvm usage
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

#ifndef GENERIC_NVM_H
#define GENERIC_NVM_H

#include <Arduino.h>
#include "../status/debug.h"
#include "eeprom_addresses.h"
#include "../compile_flags/compile_flags.h"
#include "../osc_err/osc_err.h"

// defualt size for nvm storage
#define DEFAULT_NVM_SIZE 0U

// binary for one byte
#define ONE_BYTE 0b11111111

// result of NVM initialization
enum NVMStartCode {NVM_OK, NVM_STARTED, NVM_FAILED, NVM_INVALID_SIZE};

// result of NVM reset
enum NVMDefaultCode {NVM_DEFAULT_OK, NVM_DEFAULT_SIZE_TOO_BIG,
		NVM_DEFAULT_FAIL_WRITE, NVM_DEFAULT_FAIL_CLEAR, NVM_DEFAULT_FAIL_MAX_SIZE, 
		NVM_DEFAULT_FAIL_STOP, NVM_DEFAULT_FAIL_INIT};

// variable type for retrieving values
enum VarType {
	VAR_INVALID,
	VAR_BOOL,
	VAR_INT8, VAR_UINT8,
	VAR_INT16, VAR_UINT16,
	VAR_INT32, VAR_UINT32,
	VAR_INT64, VAR_UINT64,
	VAR_FLOAT, VAR_DOUBLE,
	VAR_CHAR_ARRAY, VAR_STRING
};

// end of char array code
#define END_OF_CHAR '\0'
// max length of char array
#define CHAR_LEN_ERROR UINT8_MAX

#define CAN_DEFAULT true
#define CAN_NOT_DEFAULT false

#define DEFAULT_INT 0
#define DEFAULT_BOOL false
#define DEFAULT_FLOAT NAN

typedef uint16_t nvm_size_t;
#define NVM_MAX_SIZE_BYTES sizeof(nvm_size_t)
#define NVM_OVER_SIZE (~((nvm_size_t)0U))
#define NVM_MAX_SIZE NVM_OVER_SIZE ^ (nvm_size_t)(1U)

#ifndef NO_CHAR_ARRAY_SUPPORT

/**
 * Tests if two char arrays are the same value
 * 
 * @param first first char array to test
 * @param second second char array to test
 * 
 * @return if they are the same
 */
bool sameString(char* first, char* second);

/**
 * Tests if char array is a null pointer
 * 
 * @param value array to test
 * 
 * @return if array isn't null
 */
bool validCharPointer(char* value);

/**
 * Gets the size of a char array within 254 including '\0'
 * 
 * @param value array to test
 * 
 * @return size of array or error
 */
uint8_t charArraySize(char* value);

#endif

#ifdef __NVM_DEBUG__

/**
 * Prints the variable type inputted to console
 * 
 * @param varType type of variable
 */
void printVarType(enum VarType varType);

#endif

/****************************
 * General NVM Methods
****************************/

/**
 * Initializes NVM for operation
 * 
 * @param nvmSize size in bytes of nvm storage
 * 
 * @return code from trying to initialize
 */
enum NVMStartCode nvmInit(nvm_size_t nvmSize);

// pointer for nvm init methods
typedef NVMStartCode (*nvmInitPTR) (nvm_size_t);

/**
 * Gets max size of nvm method
 * 
 * @param size amount of bytes in nvm
 * 
 * @return if retrieval was successful
 */
bool nvmMaxSize(nvm_size_t *size);

/**
 * Resets nvm settings to default settings
 * 
 * @warning WILL CLEAR NVM DATA
 * 
 * @return code from setting default values
 */
NVMDefaultCode nvmSetDefaults(void);

/**
 * Writes critical values for nvm
 * 
 * @param nvmMaxValue max size of nvm
 * 
 * @note will reset critical values
 * 
 * @return code resulting from operations
 */
NVMDefaultCode nvmSetCritDefaults(nvm_size_t nvmMaxValue);

/**
 * Sets variables needed for general or test environment
 * 
 * @note will reset environment values
 * 
 * @return code resulting from operations
 */
NVMDefaultCode nvmSetEnvDefaults(void);

/**
 * Prints debug messages for setting nvm defaults
 * 
 * @param message message to print
 */
void printDefaultDebug(const __FlashStringHelper *message);

/****************************
 * NVM Write Methods
****************************/

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, bool value);

// pointer for nvm write bool methods
typedef bool (*nvmWriteBoolPTR) (nvm_size_t, bool);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, int8_t value);

// pointer for nvm write i8 methods
typedef bool (*nvmWriteI8PTR) (nvm_size_t, int8_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, uint8_t value);

// pointer for nvm write u8 methods
typedef bool (*nvmWriteU8PTR) (nvm_size_t, uint8_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, int16_t value);

// pointer for nvm write i16 methods
typedef bool (*nvmWriteI16PTR) (nvm_size_t, int16_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, uint16_t value);

// pointer for nvm write u16 methods
typedef bool (*nvmWriteU16PTR) (nvm_size_t, uint16_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, int32_t value);

// pointer for nvm write i32 methods
typedef bool (*nvmWriteI32PTR) (nvm_size_t, int32_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, uint32_t value);

// pointer for nvm write u32 methods
typedef bool (*nvmWriteU32PTR) (nvm_size_t, uint32_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, int64_t value);

// pointer for nvm write i64 methods
typedef bool (*nvmWriteI64PTR) (nvm_size_t, int64_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, uint64_t value);

// pointer for nvm write u64 methods
typedef bool (*nvmWriteU64PTR) (nvm_size_t, uint64_t);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, float value);

// pointer for nvm write float methods
typedef bool (*nvmWriteFloatPTR) (nvm_size_t, float);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, double value);

// pointer for nvm write double methods
typedef bool (*nvmWriteDoublePTR) (nvm_size_t, double);

#ifndef NO_CHAR_ARRAY_SUPPORT

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param maxLength maximum length of string to write
 * 
 * @return if write was valid
 */
bool nvmWriteValue(nvm_size_t key, char* value, uint8_t maxLength);

// pointer for nvm write char array methods
typedef bool (*nvmWriteCharArrayPTR) (nvm_size_t, char*, uint8_t);

#endif

/****************************
 * NVM Get Methods
****************************/

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, bool *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get bool methods
typedef bool (*nvmGetBoolPTR) (nvm_size_t, bool*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, int8_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get i8 methods
typedef bool (*nvmGetI8PTR) (nvm_size_t, int8_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, uint8_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get u8 methods
typedef bool (*nvmGetU8PTR) (nvm_size_t, uint8_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, int16_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get i16 methods
typedef bool (*nvmGetI16PTR) (nvm_size_t, int16_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, uint16_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get u16 methods
typedef bool (*nvmGetU16PTR) (nvm_size_t, uint16_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, int32_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get i32 methods
typedef bool (*nvmGetI32PTR) (nvm_size_t, int32_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, uint32_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get u32 methods
typedef bool (*nvmGetU32PTR) (nvm_size_t, uint32_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, int64_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get i64 methods
typedef bool (*nvmGetI64PTR) (nvm_size_t, int64_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, uint64_t *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get u64 methods
typedef bool (*nvmGetU64PTR) (nvm_size_t, uint64_t*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, float *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get float methods
typedef bool (*nvmGetFloatPTR) (nvm_size_t, float*, bool);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetValue(nvm_size_t key, double *value, bool canDefault = CAN_DEFAULT);

// pointer for nvm get double methods
typedef bool (*nvmGetDoublePTR) (nvm_size_t, double*, bool);

#ifndef NO_CHAR_ARRAY_SUPPORT

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * @param maxLength maximum length of string to get
 * 
 * @return if write was valid
 */
bool nvmGetValue(nvm_size_t key, char* value, uint8_t maxLength);

// pointer for nvm get char array methods
typedef bool (*nvmGetCharArrayPTR) (nvm_size_t, char*, uint8_t);

#endif

#endif