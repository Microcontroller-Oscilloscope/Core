/*
	common_thread.h - common thread functions for microcontrollers
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

#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include "../compile_flags/compile_flags.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Delays microcontroller for milliseconds
 * 
 * @param delayAmount amount to delay
 */
void hardDelayMS(uint32_t delayAmount);

/**
 * Delays microcontroller for microseconds
 * 
 * @param delayAmount amount to delay
 */
void hardDelayUS(uint32_t delayAmount);

/**
 * Starts a thread safe operation
 * 
 * @warning MUST call 'endThreadSafety'
 * 
 * @return if threads were locked and weren't already locked
 */
bool startThreadSafety(void);

/**
 * Ends a thread safe operation
 * 
 * @note intended to be called after 'startThreadSafety'
 * 
 * @return if threads were unlocked and weren't already unlocked
 */
bool endThreadSafety(void);

#ifdef __cplusplus
}
#endif

#endif