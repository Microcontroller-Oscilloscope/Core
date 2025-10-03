/*
	debug.c - debug statement formatter
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
#include "../comm/hard_serial/hard_serial.h"

memCharString leftBracket[] = {"["};
memCharString rightBracket[] = {"]:"};

memCharString errorTag[] = {"Err"};
memCharString nvmTag[] = {"NVM"};
memCharString critErrorTag[] = {"!!!"};

void printTag(memCharString* tag) {
	hardPrintMemCharArray(leftBracket);
	hardPrintMemCharArray(tag);
	hardPrintMemCharArray(rightBracket);
}

#ifdef __ERROR_DEBUG__
void printError(void) {
	printTag(errorTag);
}
#endif

#ifdef __NVM_DEBUG__
void printNVM(void) {
	printTag(nvmTag);
}
#endif

void printCritError(void) {
	printTag(critErrorTag);
}