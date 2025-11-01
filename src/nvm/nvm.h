/*
	nvm.h - methods for generic nvm usage
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

#ifndef NVM_H
#define NVM_H

#include "../compile_flags/compile_flags.h"

// defualt size for nvm storage
#define DEFAULT_NVM_SIZE 0U

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
	VAR_CHAR_ARRAY
};

// end of char array code
#define END_OF_CHAR '\0'
// max length of char array
#define CHAR_LEN_ERROR UINT8_MAX

#define CAN_DEFAULT true
#define CAN_NOT_DEFAULT false

#include <math.h>

#define DEFAULT_INT 0
#define DEFAULT_BOOL false
#define DEFAULT_FLOAT NAN

typedef uint16_t nvm_size_t;
#define NVM_MAX_SIZE_BYTES sizeof(nvm_size_t)
#define NVM_OVER_SIZE (~((nvm_size_t)0U))
#define NVM_MAX_SIZE NVM_OVER_SIZE ^ (nvm_size_t)(1U)

#ifdef __cplusplus
extern "C" {
#endif

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

#endif

/**
 * Tests if char array is a null pointer
 * 
 * @param value array to test
 * 
 * @return if array isn't null
 */
bool validCharPointer(const char* value);

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
enum NVMDefaultCode nvmSetDefaults(void);

/**
 * Writes critical values for nvm
 * 
 * @param nvmMaxValue max size of nvm
 * 
 * @note will reset critical values
 * 
 * @return code resulting from operations
 */
enum NVMDefaultCode nvmSetCritDefaults(nvm_size_t nvmMaxValue);

/**
 * Sets variables needed for general or test environment
 * 
 * @note will reset environment values
 * 
 * @return code resulting from operations
 */
enum NVMDefaultCode nvmSetEnvDefaults(void);

/****************************
 * NVM Write Methods
****************************/

/**
 * Writes bool to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteBool(nvm_size_t key, bool value);

/**
 * Writes i8 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteI8(nvm_size_t key, int8_t value);

/**
 * Writes ui8 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteUI8(nvm_size_t key, uint8_t value);

/**
 * Writes i16 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteI16(nvm_size_t key, int16_t value);

/**
 * Writes ui16 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteUI16(nvm_size_t key, uint16_t value);

/**
 * Writes i32 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteI32(nvm_size_t key, int32_t value);

/**
 * Writes ui32 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteUI32(nvm_size_t key, uint32_t value);

/**
 * Writes i64 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteI64(nvm_size_t key, int64_t value);

/**
 * Writes ui64 to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteUI64(nvm_size_t key, uint64_t value);

/**
 * Writes float to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteFloat(nvm_size_t key, float value);

/**
 * Writes double to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteDouble(nvm_size_t key, double value);

#ifndef NO_CHAR_ARRAY_SUPPORT

	/**
	 * Writes char array to nvm
	 * 
	 * @param key key of nvm address
	 * @param value value to write to nvm
	 * @param maxLength maximum length of string to write
	 * 
	 * @return if write was valid
	 */
	bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength);

#endif

/****************************
 * NVM Get Methods
****************************/

/**
 * Gets bool from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault);

/**
 * Gets i8 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault);

/**
 * Gets ui8 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault);

/**
 * Gets i16 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault);

/**
 * Gets ui16 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault);

/**
 * Gets i32 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault);

/**
 * Gets ui32 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault);

/**
 * Gets i64 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault);

/**
 * Gets ui64 from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault);

/**
 * Gets float from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault);

/**
 * Gets double from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * @param canDefault whether value should be default or not
 * 
 * @return if get was successful
 */
bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault);

#ifndef NO_CHAR_ARRAY_SUPPORT

	/**
	 * Gets char array from nvm
	 * 
	 * @param key key of nvm address
	 * @param value value to write to nvm
	 * @param maxLength maximum length of string to get
	 * 
	 * @return if write was valid
	 */
	bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength);

#endif

#ifdef __cplusplus
}
#endif

#endif