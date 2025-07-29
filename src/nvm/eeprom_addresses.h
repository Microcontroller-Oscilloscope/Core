/*
	debug.h - stores addresses for EEPROM operations
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

#ifndef EEPROMADDRESSES_H
#define EEPROMADDRESSES_H

#include <Arduino.h>

#define BYTE1_SIZE 1U
#define BYTE2_SIZE 2U
#define BYTE4_SIZE 4U
#define BYTE8_SIZE 8U

/****************************
 * Network Config
 * 
 * NOTE: the size includes the terminating
 * character to the char array
****************************/

// max string size for network SSID(name)
#ifndef SSID_STRING_SIZE
#define SSID_STRING_SIZE 32U
#endif

// max string size for network password
#ifndef PASS_STRING_SIZE
#define PASS_STRING_SIZE 16U
#endif

/****************************
 * NVM Methods
****************************/

// EEPROM version
#define EEPROM_VERSION_SIZE BYTE1_SIZE
#define EEPROM_VERSION_KEY 0U
#define EEPROM_VERSION 33

// network credentials
#define SSID_SIZE BYTE1_SIZE * SSID_STRING_SIZE

#endif