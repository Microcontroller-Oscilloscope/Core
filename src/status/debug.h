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

#ifndef DEBUG_H
#define DEBUG_H

#include "compile_flags/compile_flags.h"
#include <WString.h>

/**
 * Prints formatted tag to serial monitor '[tag]:'
 * 
 * @param tag tag to print
 * @return void
 */
void printTag(memCharString* tag);

#ifdef __ERROR_DEBUG__
/**
 * Prints error tag '[Err]:'
 * 
 * @return void
 */
void printError(void);
#endif

#ifdef __NVM_DEBUG__
/**
 * Prints nvm tag '[NVM]:'
 * 
 * @return void
 */
void printNVM(void);
#endif

/**
 * Prints critical error tag '[!!!]:'
 */
void printCritError(void);

#endif