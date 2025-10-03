/*
	status.cpp - toggle status light
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

#include "status.h"
#include <Arduino.h>
#include "../compile_flags/compile_flags.h"
#include "../hard_timer.h"

// if status LEDs are enabled
#define STATUS_LED_DEFINED() (defined(STATUS_LED_PIN) || defined(EXTERNAL_STATUS_LED_PIN)) && defined(HARD_TIMER_ID_LED) && !defined(__TEST_CASES__)

#if STATUS_LED_DEFINED()

	#define HARD_TIMER_LED HARD_TIMER(HARD_TIMER_LED_INDEX) // hardware timer for status LEDs
	#define HARD_TIMER_LED_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_LED_INDEX) // starter function for status LED
	#define HARD_TIMER_LED_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_LED_INDEX) // reference for status LED function

	bool ledToggle = false; // LED toggle state

	/**
	 * Blinks LEDs on and off
	 */
	HARD_TIMER_LED_FUNCTION() {
		ledToggle = !ledToggle;
		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, ledToggle);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, ledToggle);
		#endif
		HARD_TIMER_END();
	}

#endif

void initStatus(void) {

	#ifdef STATUS_LED_PIN
		pinMode(STATUS_LED_PIN, OUTPUT);
	#endif
	#ifdef EXTERNAL_STATUS_LED_PIN
		pinMode(EXTERNAL_STATUS_LED_PIN, OUTPUT);
	#endif
	
	#if STATUS_LED_DEFINED()
		initHardTimer(HARD_TIMER_LED, &HARD_TIMER_LED_REFERENCE, HARD_TIMER_LED_SCALAR);
	#endif
}

void setStatus(STATUS_CODE status) {

	#if STATUS_LED_DEFINED()

		// reset timers and LEDs
		cancelHardTimer(HARD_TIMER_LED);

		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, LOW);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, LOW);
		#endif

		ledToggle = false;

		// sets LEDs
		if (status == BOARD_OK) {
			#ifdef STATUS_LED_PIN
				digitalWrite(STATUS_LED_PIN, HIGH);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				digitalWrite(EXTERNAL_STATUS_LED_PIN, HIGH);
			#endif
		}
		else if (status == BOARD_CONNECTING) {
			setHardTimer(HARD_TIMER_LED, &HARD_TIMER_LED_REFERENCE, HARD_TIMER_LED_SCALAR, HARD_TIMER_LED_TICK_MULTIPLIER * CONNECTING_DELAY);
		}
		else if (status == BOARD_CRIT_ERROR) {
			setHardTimer(HARD_TIMER_LED, &HARD_TIMER_LED_REFERENCE, HARD_TIMER_LED_SCALAR, HARD_TIMER_LED_TICK_MULTIPLIER * CRIT_ERROR_DELAY);
		}

	#else
		
		if (status == BOARD_OK) {
			#ifdef STATUS_LED_PIN
				digitalWrite(STATUS_LED_PIN, HIGH);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				digitalWrite(EXTERNAL_STATUS_LED_PIN, HIGH);
			#endif
		}

	#endif
}