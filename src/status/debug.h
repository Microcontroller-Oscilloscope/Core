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
#include "../comm/hard_serial/hard_serial.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints formatted tag to serial monitor '[tag]:'
 * 
 * @warning ONLY USE WITH memCharString CONSTANTS
 * @warning ex:
 * @warning memCharString myVariable[] = {"myValue"};
 * @warning printTag(myVariable)
 * 
 * @param tag tag to print
 */
void printTag(memCharString* tag);

#ifdef __ERROR_DEBUG__
	/**
	 * Prints error tag '[Err]:'
	 */
	void printError(void);

	/**
	 * Prints error tag '[Err]:' and custom statements
	 * 
	 * @param params functions to use after printing tag
	 */
	#define PRINT_ERROR_DEBUG(params) printError(); params;
#else
	/**
	 * Prints error tag '[Err]:' and custom statements
	 * 
	 * @param params functions to use after printing tag
	 */
	#define PRINT_ERROR_DEBUG(params)
#endif

#ifdef __NVM_DEBUG__
	/**
	 * Prints nvm tag '[NVM]:'
	 */
	void printNVM(void);

	/**
	 * Prints nvm debug tag '[NVM]:' and custom statements
	 * 
	 * @param params functions to use after printing tag
	 */
	#define PRINT_NVM_DEBUG(params) printNVM(); params;
#else
	/**
	 * Prints nvm debug tag '[NVM]:' and custom statements
	 * 
	 * @param params functions to use after printing tag
	 */
	#define PRINT_NVM_DEBUG(params)
#endif

/**
 * Prints critical error tag '[!!!]:'
 */
void printCritError(void);

/**
 * Prints critical error tag '[!!!]:' and custom statements
 * 
 * @param params functions to use after printing tag
 */
#define PRINT_CRIT_ERROR(params) printCritError(); params;

#ifdef __cplusplus
}
#endif

#endif