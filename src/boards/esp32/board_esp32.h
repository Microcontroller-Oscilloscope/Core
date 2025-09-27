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

	// available hardware timers
	enum HARDWARE_TIMER_T { // hardware timer type
		HARD_TIMER0, // hardware timer 0, 64 bit counter
		HARD_TIMER1, // hardware timer 1, 64 bit counter
		HARD_TIMER2, // hardware timer 2, 64 bit counter
		HARD_TIMER3, // hardware timer 3, 64 bit counter
	};
	typedef HARDWARE_TIMER_T hardware_timer_t; // hardware timer type

	// available pre scalars
	typedef uint16_t prescalar_t; // pre scalar type

	// other timer definitions
	typedef uint64_t timertick_t; // timer tick type
	typedef void (*hard_timer_function_ptr_t) (void); // timer callback function pointer

	// timer references
	#define HARD_TIMER0_REFERENCE hardTimer0Function // reference for timer 0 function
	#define HARD_TIMER1_REFERENCE hardTimer1Function // reference for timer 1 function
	#define HARD_TIMER2_REFERENCE hardTimer2Function // reference for timer 2 function
	#define HARD_TIMER3_REFERENCE hardTimer3Function // reference for timer 3 function

	// timer functions

	/**
	 * Starter function for timer 0
	 * 
	 * APB_CLK = 80,000,000Hz
	 * 
	 * T_DER = desired time (s)
	 * 
	 * T_DER = timerTicks * (scalar / APB_CLK)
	 * 
	 * @note HARD_TIMER0_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 64-bit counter
	 * @warning 64-bit scalar
	 */
	#define HARD_TIMER0_FUNCTION() void IRAM_ATTR HARD_TIMER0_REFERENCE()

	/**
	 * Starter function for timer 1
	 * 
	 * APB_CLK = 80,000,000Hz
	 * 
	 * T_DER = desired time (s)
	 * 
	 * T_DER = timerTicks * (scalar / APB_CLK)
	 * 
	 * @note HARD_TIMER1_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 64-bit counter
	 * @warning 64-bit scalar
	 */
	#define HARD_TIMER1_FUNCTION() void IRAM_ATTR HARD_TIMER1_REFERENCE()

	/**
	 * Starter function for timer 2
	 * 
	 * APB_CLK = 80,000,000Hz
	 * 
	 * T_DER = desired time (s)
	 * 
	 * T_DER = timerTicks * (scalar / APB_CLK)
	 * 
	 * @note HARD_TIMER0_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 64-bit counter
	 * @warning 64-bit scalar
	 */
	#define HARD_TIMER2_FUNCTION() void IRAM_ATTR HARD_TIMER2_REFERENCE()

	/**
	 * Starter function for timer 3
	 * 
	 * APB_CLK = 80,000,000Hz
	 * 
	 * T_DER = desired time (s)
	 * 
	 * T_DER = timerTicks * (scalar / APB_CLK)
	 * 
	 * @note HARD_TIMER0_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 64-bit counter
	 * @warning 64-bit scalar
	 */
	#define HARD_TIMER3_FUNCTION() void IRAM_ATTR HARD_TIMER3_REFERENCE()

	#define HARD_TIMER_END() // end of function for timers

	/****************************
	 * LED Timer Config
	****************************/

	#define HARD_TIMER_LED HARD_TIMER0 // hardware timer for status LEDs
	#define HARD_TIMER_LED_FUNCTION() HARD_TIMER0_FUNCTION() // starter function for status LED
	#define HARD_TIMER_LED_REFERENCE HARD_TIMER0_REFERENCE // reference for status LED function

	#define HARD_TIMER_LED_SCALAR 80 // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER 1000 // multiplier for timer ticks

#endif
#endif