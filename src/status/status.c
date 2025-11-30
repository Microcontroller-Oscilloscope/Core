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
#include <hardware_timer.h>
#include "../osc_common/common_io.h"

// if status LEDs are enabled
#if HARD_TIMER_COUNT > 1
	#define STATUS_LED_DEFINED 1
#else
	#define STATUS_LED_DEFINED 0
#endif

#define STATUS_LED_PRIORITY HARD_TIMER_PRIORITY_DEFAULT // priority of status LED timer interrupt

inline void writeStatus(uint8_t state);

/**
 * Sets write status for given state
 * 
 * @param state whether to set pin high or low
 */
void writeStatus(uint8_t state) {
	pin_t statusPin;
	getStatusPin(&statusPin, STATUS_PIN_INTERNAL);
	if (statusPin != PIN_T_INVALID) {
		hardDigitalWrite(statusPin, state);
	}
	getStatusPin(&statusPin, STATUS_PIN_EXTERNAL);
	if (statusPin != PIN_T_INVALID) {
		hardDigitalWrite(statusPin, state);
	}
}

#if STATUS_LED_DEFINED

	volatile bool ledToggle = false; // LED toggle state
	hard_timer_enum_t ledTimer = HARD_TIMER_INVALID; // LED timer reference

	/**
	 * Blinks LEDs on and off
	 * 
	 * @param params parameter set
	 */
	void RUN_IN_RAM(ledFunction) ledFunction(void *params) {
		ledToggle = !ledToggle;
		writeStatus(ledToggle);
	}

#endif

void initStatus(void) {

	pin_t statusPin;
	getStatusPin(&statusPin, STATUS_PIN_INTERNAL);
	if (statusPin != PIN_T_INVALID) {
		hardPinMode(statusPin, PIN_MODE_OUTPUT);
	}
	getStatusPin(&statusPin, STATUS_PIN_EXTERNAL);
	if (statusPin != PIN_T_INVALID) {
		hardPinMode(statusPin, PIN_MODE_OUTPUT);
	}

	#if STATUS_LED_DEFINED
		hard_timer_claim_s priority;
		priority.slowestTimer = true;
		ledTimer = claimTimer(&priority);
	#endif
}

void setStatus(enum STATUS_CODE status) {

	#if STATUS_LED_DEFINED

		// reset timers and LEDs
		cancelHardTimer(ledTimer);
		writeStatus(DIGITAL_LOW);

		ledToggle = false;

		// sets LEDs
		if (status == BOARD_OK) {
			unclaimTimer(ledTimer);
			writeStatus(DIGITAL_HIGH);
		}
		else if (status == BOARD_CONNECTING || status == BOARD_CRIT_ERROR) {

			if (!hardTimerClaimed(ledTimer)) {
				hard_timer_claim_s priority;
				priority.slowestTimer = true;
				ledTimer = claimTimer(&priority);
			}

			uint32_t freq;

			if (status == BOARD_CONNECTING) {
				freq = CONNECTING_FREQ;
			}
			else if (status == BOARD_CRIT_ERROR) {
				freq = CRIT_ERROR_FREQ;
			}

			setHardTimer(&ledTimer, &freq, &ledFunction, NULL, STATUS_LED_PRIORITY);
		}

	#else
		
		if (status == BOARD_OK) {
			writeStatus(DIGITAL_HIGH);
		}
		else {
			writeStatus(DIGITAL_LOW);
		}

	#endif
}