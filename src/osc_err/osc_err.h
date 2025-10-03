/*
	osc_err.h - handles critical errors
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

#ifndef OSC_ERR_H
#define OSC_ERR_H

#include "osc_err_list.h"

#define OSC_ERR_DELAY 1000

/**
 * Keeps program in an error loop to alert user
 * 
 * @param err error code to display
 */
void errorLoop(enum OSC_ERR err);

#endif