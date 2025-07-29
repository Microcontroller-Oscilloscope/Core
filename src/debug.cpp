/*
	debug.h - debug statement formatter
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

#include "debug.h"

#if defined(__TAG_DEBUG__) || defined(__ERROR_DEBUG__) || defined(__NVM_DEBUG__)

void printTag(const __FlashStringHelper * tag) {
	Serial.print(F("["));
	Serial.print(tag);
	Serial.print(F("]:"));
}

#endif

#ifdef __ERROR_DEBUG__
void printError(void) {
	printTag(F("Err"));
}
#endif

#ifdef __NVM_DEBUG__
void printNVM(void) {
	printTag(F("NVM"));
}
#endif