/*
	board_pico.h - configuration flags for all Raspberry Pi Picos
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

#ifndef BOARD_PICO_H
#define BOARD_PICO_H

#include "../../board_generic.h"

#ifdef PICO

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE FLASH_NVM_SIZE // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 2 // amount of CPU cores available to board
	#endif

	#ifndef STATUS_LED_PIN
		#define STATUS_LED_PIN LED_BUILTIN // pin for status LED
	#endif

	#ifndef EXTERNAL_LED_PIN
		#define EXTERNAL_STATUS_LED_PIN 22 // pin for external status LED
	#endif

	/****************************
	 * NVM Config
	****************************/

	#if !NVM_CHECK()
		#ifndef __NVM_BEGIN__
			#define __NVM_BEGIN__ // Calls begin function for NVM
		#endif
		#ifndef __NVM_BEGIN_SIZE__
			#define __NVM_BEGIN_SIZE__ // Calls begin function with NVM size
		#endif
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 16 hardware alarm timers available
	****************************/

	#include <pico/time.h>

	// available hardware timers
	enum HARDWARE_TIMER_T { // hardware timer type
		HARD_TIMER0, // hardware timer 0, 64 bit counter
		HARD_TIMER1, // hardware timer 1, 64 bit counter
		HARD_TIMER2, // hardware timer 2, 64 bit counter
		HARD_TIMER3, // hardware timer 3, 64 bit counter
		HARD_TIMER4, // hardware timer 4, 64 bit counter
		HARD_TIMER5, // hardware timer 5, 64 bit counter
		HARD_TIMER6, // hardware timer 6, 64 bit counter
		HARD_TIMER7, // hardware timer 7, 64 bit counter
		HARD_TIMER8, // hardware timer 8, 64 bit counter
		HARD_TIMER9, // hardware timer 9, 64 bit counter
		HARD_TIMER10, // hardware timer 10, 64 bit counter
		HARD_TIMER11, // hardware timer 11, 64 bit counter
		HARD_TIMER12, // hardware timer 12, 64 bit counter
		HARD_TIMER13, // hardware timer 13, 64 bit counter
		HARD_TIMER14, // hardware timer 14, 64 bit counter
		HARD_TIMER15, // hardware timer 15, 64 bit counter
	};
	typedef HARDWARE_TIMER_T hardware_timer_t; // hardware timer type

	// available pre scalars
	enum PRE_SCALAR_T {
		SCALAR_MS, // timer prescalar for milli seconds
		SCALAR_US, // timer prescalar micro seconds
	};
	typedef PRE_SCALAR_T prescalar_t; // pre scalar type

	// other timer definitions
	typedef int64_t timertick_t; // timer tick type
	typedef bool (*hard_timer_function_ptr_t) (struct repeating_timer*); // timer callback function pointer

	// timer references
	/*#define HARD_TIMER0_REFERENCE hardTimer0Function // reference for timer 0 function
	#define HARD_TIMER1_REFERENCE hardTimer1Function // reference for timer 1 function
	#define HARD_TIMER2_REFERENCE hardTimer2Function // reference for timer 2 function
	#define HARD_TIMER3_REFERENCE hardTimer3Function // reference for timer 3 function
	#define HARD_TIMER4_REFERENCE hardTimer4Function // reference for timer 4 function
	#define HARD_TIMER5_REFERENCE hardTimer5Function // reference for timer 5 function
	#define HARD_TIMER6_REFERENCE hardTimer6Function // reference for timer 6 function
	#define HARD_TIMER7_REFERENCE hardTimer7Function // reference for timer 7 function
	#define HARD_TIMER8_REFERENCE hardTimer8Function // reference for timer 8 function
	#define HARD_TIMER9_REFERENCE hardTimer9Function // reference for timer 9 function
	#define HARD_TIMER10_REFERENCE hardTimer10Function // reference for timer 10 function
	#define HARD_TIMER11_REFERENCE hardTimer11Function // reference for timer 11 function
	#define HARD_TIMER12_REFERENCE hardTimer12Function // reference for timer 12 function
	#define HARD_TIMER13_REFERENCE hardTimer13Function // reference for timer 13 function
	#define HARD_TIMER14_REFERENCE hardTimer14Function // reference for timer 14 function
	#define HARD_TIMER15_REFERENCE hardTimer15Function // reference for timer 15 function
	*/

	#define HARD_TIMER_REFERENCE(id) hardTimerFunction##id // reference for timer #id

	// timer functions

	/**
	 * Starter function for timer #id
	 * 
	 * @param id numeric value to append to function
	 * 
	 * @note HARD_TIMER_FUNCTION({id}) {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning timerTicks: time in ms or us
	 * @warning scalar: SCALAR_MS (millis) or SCALAR_US (micros)
	 */
	#define HARD_TIMER_FUNCTION(id) bool hardTimerFunction##id(struct repeating_timer *t)

	#define HARD_TIMER_END() return true // end of function for timers

	/****************************
	 * LED Timer Config
	****************************/

	#define HARD_TIMER_LED HARD_TIMER0 // hardware timer for status LEDs
	#define HARD_TIMER_LED_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_LED) // starter function for status LED
	#define HARD_TIMER_LED_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_LED) // reference for status LED function

	#define HARD_TIMER_LED_SCALAR SCALAR_MS // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER 1 // multiplier for timer ticks

#endif

#endif