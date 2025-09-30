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

#ifndef EEPROM_ADDRESSES_H
#define EEPROM_ADDRESSES_H

#define BYTE1_SIZE 1
#define BYTE2_SIZE 2
#define BYTE4_SIZE 4
#define BYTE8_SIZE 8

/****************************
 * Network Size Config
 * 
 * NOTE: the size includes the terminating
 * character to the char array
****************************/

// max string size for network SSID (name)
#ifndef SSID_STRING_SIZE
#define SSID_STRING_SIZE 32U
#endif

// max string size for network password
#ifndef PASS_STRING_SIZE
#define PASS_STRING_SIZE 16U
#endif

/****************************
 * Critical Config
 * 
 * config needed for both normal
 * operations and unit testing
****************************/

#define CRIT_START_KEY 0

#define NVM_AVAILABLE_SIZE BYTE4_SIZE
#define NVM_AVAILABLE_KEY CRIT_START_KEY
#define NVM_AVAILABLE_DEFAULT 0U

#define CRIT_END_KEY CRIT_START_KEY + NVM_AVAILABLE_SIZE

/****************************
 * NVM Methods
****************************/

#define NVM_START_KEY CRIT_END_KEY

// NVM version
#define NVM_VERSION_SIZE BYTE2_SIZE
#define NVM_VERSION_TYPE uint16_t
#define NVM_VERSION_KEY NVM_START_KEY
#define NVM_VERSION 1

#define NVM_END_KEY NVM_VERSION_KEY + NVM_VERSION_SIZE

/****************************
 * Channel Config
****************************/

#define CH_START_KEY NVM_END_KEY

#define CH_PIN_SIZE BYTE1_SIZE
#define CH_SIZE CH_PIN_SIZE

#define CH_END_KEY CH_START_KEY + (CH_SIZE * 1)

/****************************
 * Network Config
****************************/

#define NETWORK_START_KEY CH_END_KEY

#define SSID_KEY NETWORK_START_KEY
#define PASS_KEY SSID_KEY + SSID_STRING_SIZE

// network credentials
#define SSID_SIZE BYTE1_SIZE * SSID_STRING_SIZE

#define NETWORK_END_KEY NETWORK_START_KEY + SSID_SIZE

/****************************
 * Size Config
****************************/

// nvm size needed for normal operation
#define NVM_NORMAL_SIZE NETWORK_END_KEY

#ifndef __TEST_CASES__
	#if NVM_NORMAL_SIZE > NVM_SIZE
		#error NVM_SIZE not large enough for normal operations
	#endif
#endif

#endif