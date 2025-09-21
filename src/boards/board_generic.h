/*
	board_generic.h - configuration flags for generic board
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

#ifndef BOARD_GENERIC_H
#define BOARD_GENERIC_H

// default nvm size when flash is used instead of dedicated EEPROM
#define FLASH_NVM_SIZE 4096

// checks if any default nvm methods are called
#define NVM_CHECK() (defined(__NVM_BEGIN__) || defined(__NVM_BEGIN_SIZE__) \
		|| defined(__NVM_BEGIN_RETURN__) || defined(__NVM_COMMIT__)) \
		|| !defined(__ENABLED_DEV_RELEASE__) || !defined(__ENABLED_DEV_TEST__)

/****************************
 * Checks Optional Flags
****************************/

#ifndef WAIT_RUN
	#define WAIT_RUN 0 // how long to wait for program to run in ms
#else
	#if WAIT_RUN < 0
		#error WAIT_RUN must be whole number 0 or greater in ms
	#endif
#endif

/****************************
 * Checks Custom Config Flags
****************************/

#ifndef DEFAULT_NVM
	#define DEFAULT_NVM true // whether to use the default nvm methods over custom nvm methods
#else
	#if DEFAULT_NVM != true && DEFAULT_NVM != false
		#error DEFAULT_NVM must be 'true' or 'false'
	#endif
#endif

#endif