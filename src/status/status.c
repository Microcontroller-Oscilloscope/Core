/*
	status.c - toggle status light
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
#include "../compile_flags/compile_flags.h"
#include "../hard_timer.h"
#include "../board_common.h"

// if status LEDs are enabled
#define STATUS_LED_DEFINED ((defined(STATUS_LED_PIN) || defined(EXTERNAL_STATUS_LED_PIN)) && defined(HARD_TIMER_ID_LED) && !defined(__TEST_CASES__))

#if STATUS_LED_DEFINED

	bool ledToggle = false; // LED toggle state
	hardware_timer_t ledTimer = HARD_TIMER_INVALID; // LED timer reference

	/**
	 * Blinks LEDs on and off
	 * 
	 * @param emptyParams empty parameter set
	 * 
	 * @return hard_timer_return_t: return type for timers
	 */
	hard_timer_return_t RUN_IN_RAM(ledFunction) ledFunction(hard_timer_param_t emptyParams) {
		ledToggle = !ledToggle;
		#ifdef STATUS_LED_PIN
			hardDigitalWrite(STATUS_LED_PIN, ledToggle);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			hardDigitalWrite(EXTERNAL_STATUS_LED_PIN, ledToggle);
		#endif
		HARD_TIMER_END();
	}

#endif

void initStatus(void) {

	#ifdef STATUS_LED_PIN
		hardPinMode(STATUS_LED_PIN, PIN_MODE_OUTPUT);
	#endif
	#ifdef EXTERNAL_STATUS_LED_PIN
		hardPinMode(EXTERNAL_STATUS_LED_PIN, PIN_MODE_OUTPUT);
	#endif
}

void setStatus(enum STATUS_CODE status) {

	#if STATUS_LED_DEFINED

		// reset timers and LEDs
		cancelHardTimer(ledTimer);

		#ifdef STATUS_LED_PIN
			hardDigitalWrite(STATUS_LED_PIN, DIGITAL_LOW);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			hardDigitalWrite(EXTERNAL_STATUS_LED_PIN, DIGITAL_LOW);
		#endif

		ledToggle = false;

		// sets LEDs
		if (status == BOARD_OK) {
			#ifdef STATUS_LED_PIN
				hardDigitalWrite(STATUS_LED_PIN, DIGITAL_HIGH);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				hardDigitalWrite(EXTERNAL_STATUS_LED_PIN, DIGITAL_HIGH);
			#endif
		}
		else if (status == BOARD_CONNECTING || status == BOARD_CRIT_ERROR) {

			prescalar_t scalar;
			timertick_t timerTicks;
			uint32_t freq;

			if (status == BOARD_CONNECTING) {
				freq = CONNECTING_FREQ;
			}
			else if (status == BOARD_CRIT_ERROR) {
				freq = CRIT_ERROR_FREQ;
			}

			if (getHardTimerStats(&freq, &ledTimer, &scalar, &timerTicks) != HARD_TIMER_FAIL) {
				setHardTimer(ledTimer, &ledFunction, scalar, timerTicks);
			}
		}

	#else
		
		if (status == BOARD_OK) {
			#ifdef STATUS_LED_PIN
				hardDigitalWrite(STATUS_LED_PIN, DIGITAL_HIGH);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				hardDigitalWrite(EXTERNAL_STATUS_LED_PIN, DIGITAL_HIGH);
			#endif
		}
		else {
			#ifdef STATUS_LED_PIN
				hardDigitalWrite(STATUS_LED_PIN, DIGITAL_LOW);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				hardDigitalWrite(EXTERNAL_STATUS_LED_PIN, DIGITAL_LOW);
			#endif
		}

	#endif
}