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

#if defined(PICO1W)

	#include <pico/time.h>

	struct repeating_timer ledTimer; // LED timer

	/**
	 * Blinks LEDs on and off
	 * 
	 * @param t pico timer
	 * 
	 * @return if write was successfull
	 */
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

	hw_timer_t *ledTimer = NULL; // LED timer

	#define PRE_SCALAR 80 // prescalar for hardware timer
	#define US_TO_MS 1000 // converts micro seconds to milli seconds

	/**
	 * Blinks LEDs on and off
	 */
	void IRAM_ATTR blinkLED() {
		ledToggle = !ledToggle;
		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, ledToggle);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, ledToggle);
		#endif
	}

	void timerInit() {
		ledTimer = timerBegin(LED_TIMER, PRE_SCALAR, true);
		timerAttachInterrupt(ledTimer, &blinkLED, true);
	}

	void cancelLEDTimer() {
		timerAlarmDisable(ledTimer);
		timerStop(ledTimer);
	}

	void setLEDTimer(uint16_t delayTime) {
		uint64_t timerTicks = US_TO_MS * delayTime;
		timerAlarmWrite(ledTimer, timerTicks, true);
		timerAlarmEnable(ledTimer);
		timerStart(ledTimer);
	}

#elif defined(UNOR3)

	#define COMPARE_MULTIPLIER (F_CPU / 1000000L) // value to mutliply by ms delay

	/**
	 * Blinks LEDs on and off
	 */
	ISR (TIMER1_COMPA_vect) {
		ledToggle = !ledToggle;
		#ifdef STATUS_LED_PIN
			digitalWrite(STATUS_LED_PIN, ledToggle);
		#endif
		#ifdef EXTERNAL_STATUS_LED_PIN
			digitalWrite(EXTERNAL_STATUS_LED_PIN, ledToggle);
		#endif
	}

	void timerInit() {}

	void cancelLEDTimer() {
		cancelHardTimer(HARD_TIMER_LED);
	}

	void setLEDTimer(uint16_t delayTime) {
		setHardTimer(HARD_TIMER_LED, SCALAR_1024, COMPARE_MULTIPLIER * delayTime);
	}

#else // no LED timers enabled

	void timerInit() {}
	void cancelLEDTimer() {}
	void setLEDTimer(uint16_t delayTime) {}

#endif
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
		timerInit();
	#endif
}

void setStatus(STATUS_CODE status) {

	#if STATUS_LED_DEFINED()

		cancelLEDTimer();
		resetLEDs();
		if (status == BOARD_OK) {
			#ifdef STATUS_LED_PIN
				digitalWrite(STATUS_LED_PIN, HIGH);
			#endif
			#ifdef EXTERNAL_STATUS_LED_PIN
				digitalWrite(EXTERNAL_STATUS_LED_PIN, HIGH);
			#endif
		}
		else if (status == BOARD_CONNECTING) {
			setLEDTimer(CONNECTING_DELAY);
		}
		else if (status == BOARD_CRIT_ERROR) {
			setLEDTimer(CRIT_ERROR_DELAY);
		}

	#endif
}