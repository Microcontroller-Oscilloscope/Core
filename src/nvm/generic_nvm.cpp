/*
	generic_nvm.cpp - methods for Preferences usage
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

bool validCharPointer(char* value) {
	if (value == nullptr) {
		#ifdef __NVM_DEBUG__
			printNVM();
			Serial.println(F("Can't use null pointer"));
		#endif
		return false;
	}
	return true;
}

uint8_t charArraySize(char* value) {

	if (value == nullptr) {
		return 0U;
	}

	for (uint8_t i = 0U; i < CHAR_LEN_ERROR - 1; i++) {
		if (value[i] == END_OF_CHAR) {
			return i + 1U;
		}
	}

	#ifdef __NVM_DEBUG__
		printNVM();
		Serial.println(F("Max length not long enough"));
	#endif
	return CHAR_LEN_ERROR;
}

#ifdef __NVM_DEBUG__

void printVarType(enum VarType varType) {
	switch(varType) {
		case VAR_BOOL:
			Serial.print(F("b"));
		break;
		case VAR_INT8:
			Serial.print(F("i8"));
		break;
		case VAR_UINT8:
			Serial.print(F("iu8"));
		break;
		case VAR_INT16:
			Serial.print(F("i16"));
		break;
		case VAR_UINT16:
			Serial.print(F("iu16"));
		break;
		case VAR_INT32:
			Serial.print(F("i32"));
		break;
		case VAR_UINT32:
			Serial.print(F("iu32"));
		break;
		case VAR_INT64:
			Serial.print(F("i64"));
		break;
		case VAR_UINT64:
			Serial.print(F("iu64"));
		break;
		case VAR_FLOAT:
			Serial.print(F("flt"));
		break;
		case VAR_DOUBLE:
			Serial.print(F("dbl"));
		break;
		case VAR_CHAR_ARRAY:
			Serial.print(F("char array"));
		break;
		case VAR_STRING:
			Serial.print(F("string"));
		break;
		default:
			Serial.print(F("invalid"));
		break;
	}
}

#endif