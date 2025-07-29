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

#ifndef GENERICNVM_H
#define GENERICNVM_H

#include <Arduino.h>
#include "debug.h"

#define DEFAULT_NVM_SIZE 0U

#define LEAST_BYTE 0b11111111
#define SECOND_LEAST_BYTE 0b1111111100000000
#define SECOND_MOST_BYTE 0b111111110000000000000000
#define MOST_BYTE 0b11111111000000000000000000000000

// result of NVM initialization
enum NVMStartCode {NVM_OK, NVM_STARTED, NVM_FAILED, NVM_INVALID_SIZE};

// variable type for retrieving values
enum VarType {
	VAR_INVALID,
	VAR_BOOL,
	VAR_INT8, VAR_UINT8,
	VAR_INT16, VAR_UINT16,
	VAR_INT32, VAR_UINT32,
	VAR_INT64, VAR_UINT64,
	VAR_FLOAT, VAR_DOUBLE
};

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
enum NVMStartCode nvmInit(uint16_t nvmSize);

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
bool nvmWriteValue(uint16_t key, bool value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, int8_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, uint8_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, int16_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, uint16_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, int32_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, uint32_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, int64_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, uint64_t value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, float value);

/**
 * Writes value to nvm
 * 
 * @param key key of nvm address
 * @param value value to write to nvm
 * 
 * @return if write was valid
 */
bool nvmWriteValue(uint16_t key, double value);

/****************************
 * NVM Get Methods
****************************/

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, bool *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, int8_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, uint8_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, int16_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, uint16_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, int32_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, uint32_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, int64_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, uint64_t *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, float *value);

/**
 * Gets value from nvm
 * 
 * @param key key of nvm address
 * @param value variable to store result to
 * 
 * @return if get was successful
 */
bool nvmGetValue(uint16_t key, double *value);

#endif