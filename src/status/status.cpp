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

	#define COMPARE_MULTIPLIER (F_CPU / 1000000U) // value to mutliply by ms delay

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

	enum HARDWARE_TIMER {
		HARD_TIMER0, // hardware timer 0, 8 bit counter
		HARD_TIMER1, // hardware timer 1, 16 bit counter
		HARD_TIMER2, // hardware timer 2, 8 bit counter
	};

	enum PRE_SCALAR {
		SCALAR_STOP, // stops timer
		SCALAR_1, // timer prescalar of 1, timers 0-2
		SCALAR_8, // timer prescalar of 8, timers 0-2
		SCALAR_32, // timer prescalar of 32, timer 2
		SCALAR_64, // timer prescalar of 64, timers 0-2
		SCALAR_128, // timer prescalar of 128, timer 2
		SCALAR_256, // timer prescalar of 256, timers 0-2
		SCALAR_1024, // timer prescalar of 1024, timers 0-2
	};

	void setTimer(HARDWARE_TIMER timer, PRE_SCALAR scalar, uint16_t compare_count) {

		if ((timer == HARD_TIMER0 || timer == HARD_TIMER2) && compare_count >= UINT8_MAX) {
			compare_count = UINT8_MAX - 1;
		}
		if ((timer == HARD_TIMER0 || timer == HARD_TIMER1) && (scalar == SCALAR_32 || scalar == SCALAR_128)) {
			return;
		}

		if (timer == HARD_TIMER0) {
			cli();
			TCCR0A = 0;
			TCCR0B = 0;
			TCNT0 = 0;
			OCR0A = compare_count;
			TCCR0A |= (1 << WGM01);
			if (scalar == SCALAR_STOP) {
				TCCR0B &= ~(0b00000000 | ((1 << CS00) | (1 << CS01) | (1 << CS02)));
			}
			else {
				if (scalar == SCALAR_1 || scalar == SCALAR_64 || scalar == SCALAR_1024) {
					TCCR0B |= (1 << CS00);
				}
				if (scalar == SCALAR_8 || scalar == SCALAR_64) {
					TCCR0B |= (1 << CS01);
				}
				if (scalar == SCALAR_256 || scalar == SCALAR_1024) {
					TCCR0B |= (1 << CS02);
				}
			}
			TIMSK0 |= (1 << OCIE0A);
			sei();
		}
		else if (timer == HARD_TIMER1) {
			cli();
			TCCR1A = 0;
			TCCR1B = 0;
			TCNT1 = 0;
			OCR1A = compare_count;
			TCCR1B |= (1 << WGM12);
			if (scalar == SCALAR_STOP) {
				TCCR1B &= ~(0b00000000 | ((1 << CS10) | (1 << CS11) | (1 << CS12)));
				TIMSK1 &= ~(0b00000000 | (1 << OCIE1A));
			}
			else {
				if (scalar == SCALAR_1 || scalar == SCALAR_64 || scalar == SCALAR_1024) {
					TCCR1B |= (1 << CS10);
				}
				if (scalar == SCALAR_8 || scalar == SCALAR_64) {
					TCCR1B |= (1 << CS11);
				}
				if (scalar == SCALAR_256 || scalar == SCALAR_1024) {
					TCCR1B |= (1 << CS12);
				}
				TIMSK1 |= (1 << OCIE1A);
			}
			sei();
		}
		else if (timer == HARD_TIMER2) {
			cli();
			sei();
		}
	}

	void timerInit() {}

	void cancelLEDTimer() {
		setTimer(HARD_TIMER1, SCALAR_STOP, 0U);
	}

	void setLEDTimer(uint16_t delayTime) {
		setTimer(HARD_TIMER1, SCALAR_1024, COMPARE_MULTIPLIER * delayTime);
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