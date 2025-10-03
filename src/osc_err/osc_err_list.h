/*
	osc_err_list.h - lists critical errors
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

#ifndef OSC_ERR_LIST_H
#define OSC_ERR_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * List of errors that hang microcontroller
 */
enum OSC_ERR {

	// 0-9
	NVM_INIT_FAIL, // NVM failed to initialize
	NVM_WRITE_FAIL, // NVM failed write operation
	NVM_GET_FAIL, // NVM failed get operation
	NVM_NULL_POINTER_FAIL, // NVM function pointer was null
	NVM_SIZE_TOO_BIG_FAIL, // NVM_SIZE in board consts is over max
	NVM_SIZE_TOO_SMALL_FAIL, // NVM_SIZE in board consts is too small for data
};

#ifdef __cplusplus
}
#endif

#endif