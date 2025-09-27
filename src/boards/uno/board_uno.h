/*
	board_uno.h - configuration flags for Arduino Uno
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

#ifndef BOARD_UNO_H
#define BOARD_UNO_H

#include "../board_generic.h"

#ifdef ARDUINO_AVR_UNO
	#define UNOR3
	#define BOARD_FOUND
#endif

#ifdef UNOR3

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE 1024 // size in bytes of NVM
	#endif

	#ifndef CORE_COUNT
		#define CORE_COUNT 1 // amount of CPU cores available to board
	#endif

	#ifndef STATUS_LED_PIN
		#define STATUS_LED_PIN 13 // pin for status LED
	#endif

	/****************************
	 * NVM Config
	****************************/

	#if !NVM_CHECK()
	#endif

	/****************************
	 * Timer Config
	 * 
	 * Only 3 hardware timers available
	****************************/

	// available hardware timers
	enum HARDWARE_TIMER_T {
		HARD_TIMER0, // hardware timer 0, 8 bit counter
		HARD_TIMER1, // hardware timer 1, 16 bit counter
		HARD_TIMER2, // hardware timer 2, 8 bit counter
	};
	typedef HARDWARE_TIMER_T hardware_timer_t; // hardware timer type

	// available pre scalars
	enum PRE_SCALAR_T {
		SCALAR_1, // timer prescalar of 1, timers 0-2
		SCALAR_8, // timer prescalar of 8, timers 0-2
		SCALAR_32, // timer prescalar of 32, timer 2
		SCALAR_64, // timer prescalar of 64, timers 0-2
		SCALAR_128, // timer prescalar of 128, timer 2
		SCALAR_256, // timer prescalar of 256, timers 0-2
		SCALAR_1024, // timer prescalar of 1024, timers 0-2
	};
	typedef PRE_SCALAR_T prescalar_t; // pre scalar type

	// other timer definitions
	typedef uint16_t timertick_t; // timer tick type
	typedef void (*hard_timer_function_ptr_t) (void); // timer callback function pointer

	// timer references
	#define HARD_TIMER0_REFERENCE VOID_FUNCTION_NAME // reference for timer 0 function
	#define HARD_TIMER1_REFERENCE VOID_FUNCTION_NAME // reference for timer 1 function
	#define HARD_TIMER2_REFERENCE VOID_FUNCTION_NAME // reference for timer 2 function

	// timer functions

	/**
	 * Starter function for timer 0
	 * 
	 * F_CPU = 16,000,000
	 * 
	 * F_DER = desired frequency (Hz)
	 * 
	 * timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 * 
	 * @note HARD_TIMER0_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 8-bit counter
	 * @warning scalars: 1, 8, 64, 256, 1024
	 */
	#define HARD_TIMER0_FUNCTION() ISR (TIMER0_COMPA_vect)

	/**
	 * Starter function for timer 1
	 * 
	 * F_CPU = 16,000,000
	 * 
	 * F_DER = desired frequency (Hz)
	 * 
	 * timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 * 
	 * @note HARD_TIMER1_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 16-bit counter
	 * @warning scalars: 1, 8, 64, 256, 1024
	 */
	#define HARD_TIMER1_FUNCTION() ISR (TIMER1_COMPA_vect)

	/**
	 * Starter function for timer 2
	 * 
	 * F_CPU = 16,000,000
	 * 
	 * F_DER = desired frequency (Hz)
	 * 
	 * timerTicks = [F_CPU / (scalar * F_DER)] - 1
	 * 
	 * @note HARD_TIMER2_FUNCTION() {
	 * @note 	{contents}
	 * @note 	HARD_TIMER_END();
	 * @note }
	 * 
	 * @warning 8-bit counter
	 * @warning scalars: 1, 8, 32, 64, 128, 256, 1024
	 */
	#define HARD_TIMER2_FUNCTION() ISR (TIMER2_COMPA_vect)

	#define HARD_TIMER_END() // end of function for timers

	/****************************
	 * LED Timer Config
	****************************/

	#define HARD_TIMER_LED HARD_TIMER1 // hardware timer for status LEDs
	#define HARD_TIMER_LED_FUNCTION() HARD_TIMER1_FUNCTION() // starter function for status LED
	#define HARD_TIMER_LED_REFERENCE HARD_TIMER0_REFERENCE // reference for status LED function
	
	#define HARD_TIMER_LED_SCALAR SCALAR_1024 // pre scalar for LED timer
	#define HARD_TIMER_LED_TICK_MULTIPLIER (F_CPU / 1000000L) // multiplier for timer ticks

#endif
#endif