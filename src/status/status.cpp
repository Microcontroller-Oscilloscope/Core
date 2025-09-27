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

// if status LEDs are enabled
#define STATUS_LED_DEFINED() defined(STATUS_LED_PIN) || defined(EXTERNAL_STATUS_LED_PIN)

#if STATUS_LED_DEFINED()

/**
 * Sets LED timer for set amount of time
 * 
 * @param delayTime amount to delay
 */
void setLEDTimer(uint16_t delayTime);

/**
 * Cancels timer for LED when switching states
 */
void cancelLEDTimer();

/**
 * Sets up timers for each board
 */
void timerInit();

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

/*#if defined(PICO1W)

	#include <pico/time.h>

	struct repeating_timer ledTimer; // LED timer

	bool blinkLED(struct repeating_timer *t) {
		ledToggle = !ledToggle;
		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, ledToggle);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, ledToggle);
		#endif
		return true;
	}

	void timerInit() {}

	void cancelLEDTimer() {
		cancel_repeating_timer(&ledTimer);
	}

	void setLEDTimer(uint16_t delayTime) {
		add_repeating_timer_ms(delayTime, blinkLED, NULL, &ledTimer);
	}

#elif defined(ESP32DEVC)

	#define PRE_SCALAR 80 // prescalar for hardware timer
	#define US_TO_MS 1000 // converts micro seconds to milli seconds

	void timerInit() {
		initHardTimer(HARD_TIMER_LED, &HARD_TIMER_LED_REFERENCE, PRE_SCALAR);
	}
	void cancelLEDTimer() {
		cancelHardTimer(HARD_TIMER_LED);
	}
	void setLEDTimer(uint16_t delayTime) {
		uint64_t timerTicks = US_TO_MS * delayTime;
		setHardTimer(HARD_TIMER_LED, timerTicks);
	}

#elif defined(UNOR3)

	void timerInit() {
		initHardTimer(HARD_TIMER_LED, &HARD_TIMER_LED_REFERENCE, HARD_TIMER_LED_SCALAR);
	}
	void cancelLEDTimer() {
		cancelHardTimer(HARD_TIMER_LED);
	}
	void setLEDTimer(uint16_t delayTime) {
		timertick_t timerTicks = HARD_TIMER_LED_TICK_MULTIPLIER * delayTime;
		setHardTimer(HARD_TIMER_LED, HARD_TIMER_LED_SCALAR, timerTicks);
	}

#else // no LED timers enabled

	void timerInit() {}
	void cancelLEDTimer() {}
	void setLEDTimer(uint16_t delayTime) {}

#endif*/
#endif

/**
 * Resets LEDs to off
 */
void resetLEDs() {
	#ifdef STATUS_LED_PIN
		digitalWrite(STATUS_LED_PIN, LOW);
	#endif
	#ifdef EXTERNAL_STATUS_LED_PIN
		digitalWrite(EXTERNAL_STATUS_LED_PIN, LOW);
	#endif
}

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
		ledToggle = false;

		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, LOW);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, LOW);
		#endif

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
			setHardTimer(HARD_TIMER_LED, HARD_TIMER_LED_SCALAR, HARD_TIMER_LED_TICK_MULTIPLIER * CONNECTING_DELAY);
		}
		else if (status == BOARD_CRIT_ERROR) {
			setHardTimer(HARD_TIMER_LED, HARD_TIMER_LED_SCALAR, HARD_TIMER_LED_TICK_MULTIPLIER * CRIT_ERROR_DELAY);
		}

	#endif
}