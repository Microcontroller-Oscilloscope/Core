/*
	generic_nvm.c - methods for Preferences usage
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

#include "generic_nvm.h"

#include "eeprom_addresses.h"
#include "../osc_err/osc_err.h"
#include "../status/debug.h"
#include "../comm/hard_serial/hard_serial.h"

#include <stddef.h>

#ifndef NO_CHAR_ARRAY_SUPPORT

	memCharString noNull[] = {"Can't use null pointer"};
	memCharString lengthToSmall[] = {"Max length not long enough"};

	bool sameString(char* first, char* second) {
		uint8_t firstSize = charArraySize(first);
		uint8_t secondSize = charArraySize(second);

		if (firstSize != secondSize || firstSize == 0) {
			return false;
		}

		for (uint8_t i = 0; i < firstSize; i++) {
			if (first[i] != second[i]) {
				return false;
			}
		}

		return true;
	}

	bool validCharPointer(char* value) {
		if (value == NULL) {
			#ifdef __NVM_DEBUG__
				printNVM();
				hardPrintMemCharArrayln(noNull);
			#endif
			return false;
		}
		return true;
	}

#endif

#ifdef __NVM_DEBUG__

	memCharString bStr[] = {"b"};
	memCharString i8Str[] = {"i8"};
	memCharString ui8Str[] = {"ui8"};
	memCharString i16Str[] = {"i16"};
	memCharString ui16Str[] = {"ui16"};
	memCharString i32Str[] = {"i32"};
	memCharString ui32Str[] = {"ui32"};
	memCharString i64Str[] = {"i64"};
	memCharString ui64Str[] = {"ui64"};
	memCharString fStr[] = {"flt"};
	memCharString dStr[] = {"dbl"};
	memCharString caStr[] = {"char arr"};
	memCharString invStr[] = {"invalid"};

	void printVarType(enum VarType varType) {
		switch(varType) {
			case VAR_BOOL:
				hardPrintMemCharArray(bStr);
			break;
			case VAR_INT8:
				hardPrintMemCharArray(i8Str);
			break;
			case VAR_UINT8:
				hardPrintMemCharArray(ui8Str);
			break;
			case VAR_INT16:
				hardPrintMemCharArray(i16Str);
			break;
			case VAR_UINT16:
				hardPrintMemCharArray(ui16Str);
			break;
			case VAR_INT32:
				hardPrintMemCharArray(i32Str);
			break;
			case VAR_UINT32:
				hardPrintMemCharArray(ui32Str);
			break;
			case VAR_INT64:
				hardPrintMemCharArray(i64Str);
			break;
			case VAR_UINT64:
				hardPrintMemCharArray(ui64Str);
			break;
			case VAR_FLOAT:
				hardPrintMemCharArray(fStr);
			break;
			case VAR_DOUBLE:
				hardPrintMemCharArray(dStr);
			break;
			case VAR_CHAR_ARRAY:
				hardPrintMemCharArray(caStr);
			break;
			default:
				hardPrintMemCharArray(invStr);
			break;
		}
	}

#endif

memCharString failWriteStr[] = {"nvm failed write"};

void printDefaultDebug(memCharString *message) {
	#ifdef __NVM_DEBUG__
		printNVM();
		hardPrintMemCharArray(message);
	#endif
}

#define FAILED_WRITE_MESSAGE() printDefaultDebug(failWriteStr)

enum NVMDefaultCode nvmSetCritDefaults(nvm_size_t nvmMaxValue) {

	if (sizeof(nvm_size_t) == sizeof(uint8_t)) {
		if (!nvmWriteUI8(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			FAILED_WRITE_MESSAGE();
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint16_t)) {
		if (!nvmWriteUI16(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			FAILED_WRITE_MESSAGE();
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint32_t)) {
		if (!nvmWriteUI32(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			FAILED_WRITE_MESSAGE();
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else if (sizeof(nvm_size_t) == sizeof(uint64_t)) {
		if (!nvmWriteUI64(NVM_AVAILABLE_KEY, nvmMaxValue)) {
			FAILED_WRITE_MESSAGE();
			return NVM_DEFAULT_FAIL_WRITE;
		}
	}
	else {
		FAILED_WRITE_MESSAGE();
		return NVM_DEFAULT_FAIL_WRITE;
	}

	return NVM_DEFAULT_OK;
}

#ifdef __TEST_CASES__
enum NVMDefaultCode nvmSetEnvDefaults(void) {
	return NVM_DEFAULT_OK;
}
#else
enum NVMDefaultCode nvmSetEnvDefaults(void) {
	return NVM_DEFAULT_OK;
}
#endif