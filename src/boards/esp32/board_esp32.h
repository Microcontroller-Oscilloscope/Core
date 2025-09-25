/*
	board_esp32.h - configuration flags for Espressif ESP32
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

#ifndef BOARD_ESP32_H
#define BOARD_ESP32_H

#include "../board_generic.h"

#ifdef ESP32
	#define ESP32DEVC
	#define BOARD_FOUND
#endif

#ifdef ESP32DEVC

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE FLASH_NVM_SIZE // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 2 // amount of CPU cores available to board
	#endif

	#ifndef EXTERNAL_LED_PIN
		#define EXTERNAL_STATUS_LED_PIN 23 // pin for external status LED
	#endif

	/****************************
	 * NVM Config
	****************************/

	#if !NVM_CHECK()
		#ifndef __NVM_BEGIN__
			#define __NVM_BEGIN__ // Calls begin function for NVM
		#endif
		#ifndef __NVM_BEGIN_RETURN__
			#define __NVM_BEGIN_RETURN__ // Checks return parameter of nvm begin
		#endif
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 4 hardware timers available
	****************************/

	#define LED_TIMER 0 // Used hardware timer for blinking status LED

#endif
#endif