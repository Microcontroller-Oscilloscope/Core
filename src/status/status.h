/*
	status.h - toggle status light
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

#ifndef STATUS_H
#define STATUS_H

/**
 * List of status codes for LED
 */
enum STATUS_CODE {
	BOARD_OK, // board is in an operable state
	BOARD_CONNECTING, // board is trying to connect to device
	BOARD_CRIT_ERROR, // board encountered a critical error
};

#define CONNECTING_FREQ 1 // LED blink frequency for connecting
#define CRIT_ERROR_FREQ 10 // LED blink frequency for critical errors

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes status pin configuration
 */
void initStatus(void);

/**
 * Sets status of the board's LED
 * 
 * @param status status code to show
 */
void setStatus(enum STATUS_CODE status);

#ifdef __cplusplus
}
#endif

#endif