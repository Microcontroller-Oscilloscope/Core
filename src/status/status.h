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

#define CONNECTING_DELAY 1000 // LED blink delay for connecting in ms
#define CRIT_ERROR_DELAY 100 // LED blink delay for critical errors in ms

/**
 * Initializes status pin configuration
 */
void initStatus(void);

/**
 * Sets status of the board's LED
 * 
 * @param status status code to show
 */
void setStatus(STATUS_CODE status);

#endif