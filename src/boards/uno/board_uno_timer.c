/*
	board_uno_timer.c - timer configuration for Arduino Uno
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

#include "../board.h"

#ifdef UNOR3

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../hard_timer.h"

uint8_t timerStates = 0U;

#define FREQ_MAX 1000000 // max frequency user set timer can be
#define FREQ_MIN_8_COUNTER 62 // min frequency for 8 bit counter

const uint16_t scalarMask[] PROGMEM = {
	1, // SCALAR_1
	8, // SCALAR_8
	32, // SCALAR_32
	64, // SCALAR_64
	128, // SCALAR_128
	256, // SCALAR_256
	1024, // SCALAR_1024
};

#define SCALAR_MASK_SIZE (sizeof(scalarMask) / sizeof(uint16_t)) // size of scalarMask

/**
 * Gets scalar mask value from enum
 * 
 * @param i scalar enum
 * 
 * @return literal int value
 */
#define GET_MASK(i) ((uint16_t)pgm_read_word_near(scalarMask + i))

/**
 * Calculates timer frequency from scalar value and timer tick value
 * 
 * @param calcScalar scalar enum
 * @param calcTimerTicks timer ticks to count for
 * 
 * @return calculated frequency
 * 
 * @warning rounds to nearest int
 */
#define CALC_FREQ(calcScalar, calcTimerTicks) (F_CPU / ((freq_t)GET_MASK(calcScalar) * (calcTimerTicks + 1)))

/**
 * Calculates timer ticks from scalar value and frequency
 * 
 * @param calcScalar scalar enum
 * @param calcFreq frequency to get ticks for
 * 
 * @return calculated timer ticks
 * 
 * @warning rounds to nearest int
 */
#define CALC_TICKS(calcScalar, calcFreq) ((F_CPU / (GET_MASK(calcScalar) * calcFreq)) - 1)

/**
 * Calculates positive value
 * 
 * @param value value to test
 * 
 * @return positive value
 * 
 * @warning doesn't convert if value is max negative value
 */
#define abs(value) (value > 0? value : -value)

/****************************
 * Timer 0
****************************/

#define TIMER_0_COMP TCCR0A // sets compare mode
#define TIMER_0_WAVEFORM TCCR0B // sets waveform mode
#define TIMER_0_COUNTER TCNT0 // current counter value
#define TIMER_0_TARGET OCR0A // target tick value
#define TIMER_0_INCR TCCR0A // sets increment mode
#define TIMER_0_SCAL TCCR0B // sets scalar mode
#define TIMER_0_INTERR TIMSK0 // sets interrupt

#define TIMER_0_SCALAR_ENABLE ((1 << CS00) | (1 << CS01) | (1 << CS02)) // flags for timer 0 scalar
#define TIMER_0_INTERR_ENABLE (1 << OCIE0A) // flags for timer 0 interrupt
#define TIMER_0_INCREM_ENABLE (1 << WGM01) // flags for timer 0 increment

#define TIMER_0_STARTED() (!!(timerStates)) // if timer 0 was started

/**
 * sets scalar for timer 0
 * 
 * @param scalar prescalar_t type for scalar value
 */
#define TIMER_0_SET_SCALAR(scalar) \
	if (scalar == SCALAR_1 || scalar == SCALAR_64 || scalar == SCALAR_1024) { \
		TIMER_0_SCAL |= (1 << CS00); \
	} \
	if (scalar == SCALAR_8 || scalar == SCALAR_64) { \
		TIMER_0_SCAL |= (1 << CS01); \
	} \
	if (scalar == SCALAR_256 || scalar == SCALAR_1024) { \
		TIMER_0_SCAL |= (1 << CS02); \
	}

volatile void(*timer0Ptr)() = NULL;

ISR(TIMER0_COMPA_vect) {
	((void(*)())timer0Ptr)();
}

/****************************
 * Timer 1
****************************/

#define TIMER_1_COMP TCCR1A // sets compare mode
#define TIMER_1_WAVEFORM TCCR1B // sets waveform mode
#define TIMER_1_COUNTER TCNT1 // current counter value
#define TIMER_1_TARGET OCR1A // target tick value
#define TIMER_1_INCR TCCR1B // sets increment mode
#define TIMER_1_SCAL TCCR1B // sets scalar mode
#define TIMER_1_INTERR TIMSK1 // sets interrupt

#define TIMER_1_SCALAR_ENABLE ((1 << CS10) | (1 << CS11) | (1 << CS12)) // flags for timer 1 scalar
#define TIMER_1_INTERR_ENABLE (1 << OCIE1A) // flags for timer 1 interrupt
#define TIMER_1_INCREM_ENABLE (1 << WGM12) // flags for timer 1 increment

#define TIMER_1_STARTED() (!!((1 << 1) & timerStates)) // if timer 1 was started

/**
 * sets scalar for timer 1
 * 
 * @param scalar prescalar_t type for scalar value
 */
#define TIMER_1_SET_SCALAR(scalar) \
	if (scalar == SCALAR_1 || scalar == SCALAR_64 || scalar == SCALAR_1024) { \
		TIMER_1_SCAL |= (1 << CS10); \
	} \
	if (scalar == SCALAR_8 || scalar == SCALAR_64) { \
		TIMER_1_SCAL |= (1 << CS11); \
	} \
	if (scalar == SCALAR_256 || scalar == SCALAR_1024) { \
		TIMER_1_SCAL |= (1 << CS12); \
	}

volatile void(*timer1Ptr)() = NULL;

ISR(TIMER1_COMPA_vect) {
	((void(*)())timer1Ptr)();
}

/****************************
 * Timer 2
****************************/

#define TIMER_2_COMP TCCR2A // sets compare mode
#define TIMER_2_WAVEFORM TCCR2B // sets waveform mode
#define TIMER_2_COUNTER TCNT2 // current counter value
#define TIMER_2_TARGET OCR2A // target tick value
#define TIMER_2_INCR TCCR2A // sets increment mode
#define TIMER_2_SCAL TCCR2B // sets scalar mode
#define TIMER_2_INTERR TIMSK2 // sets interrupt

#define TIMER_2_SCALAR_ENABLE ((1 << CS20) | (1 << CS21) | (1 << CS22)) // flags for timer 2 scalar
#define TIMER_2_INTERR_ENABLE (1 << OCIE2A) // flags for timer 2 interrupt
#define TIMER_2_INCREM_ENABLE (1 << WGM21) // flags for timer 2 increment

#define TIMER_2_STARTED() (!!((1 << 2) & timerStates)) // if timer 2 was started

/**
 * sets scalar for timer 2
 * 
 * @param scalar prescalar_t type for scalar value
 */
#define TIMER_2_SET_SCALAR(scalar) \
	if (scalar == SCALAR_1 || scalar == SCALAR_32 || scalar == SCALAR_128 || scalar == SCALAR_1024) { \
		TIMER_2_SCAL |= (1 << CS20); \
	} \
	if (scalar == SCALAR_8 || scalar == SCALAR_32 || scalar == SCALAR_256 || scalar == SCALAR_1024) { \
		TIMER_2_SCAL |= (1 << CS21); \
	} \
	if (scalar == SCALAR_64 || scalar == SCALAR_128 || scalar == SCALAR_256 || scalar == SCALAR_1024) { \
		TIMER_2_SCAL |= (1 << CS22); \
	}

volatile void(*timer2Ptr)() = NULL;

ISR(TIMER2_COMPA_vect) {
	((void(*)())timer2Ptr)();
}

/****************************
 * Timer Functions
****************************/

/**
 * Sets timer started state
 * 
 * @param timer timer to set
 * @param state whether or not timer is started
 */
void setTimerStarted(hardware_timer_t timer, bool state) {
	if (timer >= 0 && timer < NUM_TIMERS) {
		if (state) {
			timerStates |= (1 << timer);
		}
		else {
			timerStates &= (~(1 << timer));
		}
	}
}

/**
 * Detects if scalar value is out of bounds
 * 
 * @param timer hardware_timer_t timer referenced
 * @param scalar prescalar_t scalar referenced
 */
#define SCALAR_OUT_OF_BOUNDS(timer, scalar) ((timer == HARD_TIMER0 || timer == HARD_TIMER1) && (scalar == SCALAR_32 || scalar == SCALAR_128))

/**
 * Detects if timer ticks value is out of bounds
 * 
 * @param timer hardware_timer_t timer referenced
 * @param timerTicks timertick_t timerTicks referenced
 */
#define TICKS_OUT_OF_BOUNDS(timer, timerTicks) ((timer == HARD_TIMER0 || timer == HARD_TIMER2) && timerTicks >= UINT8_MAX)

/**
 * Tests if given scalar and timer ticks equal a given frequency
 * 
 * @param freq target frequency
 * @param scalar scalar value
 * @param ticks timer tick count
 * 
 * @return if parameters generate frequency
 */
bool sameFreq(freq_t freq, prescalar_t scalar, timertick_t ticks) {

	if (F_CPU % ((freq_t)GET_MASK(scalar) * (ticks + 1)) != 0) {
		return false;
	}
	if (CALC_FREQ(scalar, ticks) != freq) {
		return false;
	}
	return true;
}

/**
 * Gets stats for any timer
 * 
 * @param freq pointer to target frequency
 * @param timer timer to test for
 * @param scalar pointer to scalar value
 * @param timerTicks pointer to timer tick count
 */
void getStats(freq_t *freq, hardware_timer_t timer, prescalar_t *scalar, timertick_t *timerTicks) {

	*scalar = SCALAR_1;
	*timerTicks = 0;
	freq_t closestFreq = 0;

	for (uint8_t i = SCALAR_MASK_SIZE - 1; i < SCALAR_MASK_SIZE; i--) {

		// ignore invalid scalars
		if ((i == SCALAR_32 || i == SCALAR_128) && timer != HARD_TIMER2) {
			continue;
		}

		// ignore invalid ticks
		if (CALC_TICKS(i, *freq) > UINT16_MAX || (CALC_TICKS(i, *freq) > UINT8_MAX && timer != HARD_TIMER1)) {
			continue;
		}
		timertick_t calcTicks = CALC_TICKS(i, *freq);

		// frequency is exact value
		if (sameFreq(*freq, i, calcTicks)) {
			*scalar = i;
			*timerTicks = calcTicks;
			return;
		}

		// test if newly calculated frequency is closer
		if (abs(*freq - closestFreq) > abs(*freq - CALC_FREQ(i, calcTicks)) || closestFreq == 0) {
			*scalar = (prescalar_t)i;
			*timerTicks = calcTicks;
			closestFreq = CALC_FREQ(i, calcTicks);
		}
	}
}

enum HardTimerStatusReturn getHardTimerStats(freq_t *freq, hardware_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks) {
	
	if (*freq > FREQ_MAX) {
		return HARD_TIMER_FREQ_OUT_OF_RANGE;
	}

	if (*freq < FREQ_MIN_8_COUNTER) {
		// calculates slow frequencies for timer 1

		if (hardTimerStarted(HARD_TIMER1)) {
			// slow timer unavailable
			return HARD_TIMER_FAIL;
		}

		getStats(&(*freq), HARD_TIMER1, &(*scalar), &(*timerTicks));

		*timer = HARD_TIMER1;

		if (sameFreq(*freq, *scalar, *timerTicks)) {
			return HARD_TIMER_OK;
		}
		else {
			return HARD_TIMER_SLIGHTLY_OFF;
		}
	}
	else {
		/**
		 * calculates frequencies for remaining timers
		 * 
		 * first checks timer 0 since its configuration is valid for all timers
		 * then checks timer 1 since it can do slower frequencies than the others
		 * finally checks timer 2 since it can be more accurate
		 */

		enum HardTimerStatusReturn status = HARD_TIMER_FAIL;

		prescalar_t tempScalar = SCALAR_1;
		timertick_t tempTicks = 0;
		freq_t tempFreq = *freq;

		// gets timer 0
		if (!hardTimerStarted(HARD_TIMER0)) {
			getStats(&tempFreq, HARD_TIMER0, scalar, timerTicks);

			if (sameFreq(tempFreq, *scalar, *timerTicks)) {
				status = HARD_TIMER_OK;
			}
			else {
				status = HARD_TIMER_SLIGHTLY_OFF;
			}

			*timer = HARD_TIMER0;
		}

		// gets timer 1
		if (!hardTimerStarted(HARD_TIMER1)) {

			if (*timerTicks != 0) {
				// timer 0 in use
				getStats(&tempFreq, HARD_TIMER1, scalar, timerTicks);
				*timer = HARD_TIMER1;

				if (sameFreq(tempFreq, *scalar, *timerTicks)) {
					status = HARD_TIMER_OK;
				}
				else {
					status = HARD_TIMER_SLIGHTLY_OFF;
				}
			}
			else {
				// timer 0 available
				freq_t calcFreq;
				getStats(&calcFreq, HARD_TIMER1, &tempScalar, &tempTicks);

				if (abs(*freq - tempFreq) > abs(*freq - calcFreq)) {
					tempFreq = calcFreq;
					*timer = HARD_TIMER1;
					*scalar = tempScalar;
					*timerTicks = tempTicks;

					if (sameFreq(*freq, *scalar, *timerTicks)) {
						status = HARD_TIMER_OK;
					}
					else {
						status = HARD_TIMER_SLIGHTLY_OFF;
					}
				}
			}
		}

		// gets timer 2
		if (!hardTimerStarted(HARD_TIMER2)) {

			if (*timerTicks != 0) {
				// timer 0 and 1 in use
				getStats(&tempFreq, HARD_TIMER2, scalar, timerTicks);
				*timer = HARD_TIMER2;

				if (sameFreq(tempFreq, *scalar, *timerTicks)) {
					status = HARD_TIMER_OK;
				}
				else {
					status = HARD_TIMER_SLIGHTLY_OFF;
				}
			}
			else {
				// timer 0 and/or 1 available
				freq_t calcFreq;
				getStats(&calcFreq, HARD_TIMER2, &tempScalar, &tempTicks);

				if (abs(*freq - tempFreq) > abs(*freq - calcFreq)) {
					tempFreq = calcFreq;
					*timer = HARD_TIMER2;
					*scalar = tempScalar;
					*timerTicks = tempTicks;

					if (sameFreq(*freq, *scalar, *timerTicks)) {
						status = HARD_TIMER_OK;
					}
					else {
						status = HARD_TIMER_SLIGHTLY_OFF;
					}
				}
			}
		}

		*freq = tempFreq;

		return status;
	}

	return HARD_TIMER_FAIL;
}

bool hardTimerStarted(hardware_timer_t timer) {

	if (timer >= 0 && timer < NUM_TIMERS) {
		return !!((1 << timer) & timerStates);
	}

	return false;
}

bool cancelHardTimer(hardware_timer_t timer) {
	if (timer == HARD_TIMER0) {
		if (TIMER_0_STARTED()) {
			cli();
			TIMER_0_SCAL &= ~TIMER_0_SCALAR_ENABLE;
			TIMER_0_INTERR &= ~TIMER_0_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, false);
			timer0Ptr = NULL;
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (TIMER_1_STARTED()) {
			cli();
			TIMER_1_SCAL &= ~TIMER_1_SCALAR_ENABLE;
			TIMER_1_INTERR &= ~TIMER_1_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, false);
			timer1Ptr = NULL;
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (TIMER_2_STARTED()) {
			cli();
			TIMER_2_SCAL &= ~TIMER_2_SCALAR_ENABLE;
			TIMER_2_INTERR &= ~TIMER_2_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, false);
			timer2Ptr = NULL;
			return true;
		}
	}

	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {

	if (TICKS_OUT_OF_BOUNDS(timer, timerTicks) || SCALAR_OUT_OF_BOUNDS(timer, scalar)) {
		return false;
	}

	if (timer == HARD_TIMER0) {
		if (!TIMER_0_STARTED()) {
			timer0Ptr = function;
			cli();
			TIMER_0_COMP = 0;
			TIMER_0_WAVEFORM = 0;
			TIMER_0_COUNTER = 0;
			TIMER_0_TARGET = timerTicks;
			TIMER_0_INCR |= TIMER_0_INCREM_ENABLE;
			TIMER_0_SET_SCALAR(scalar);
			TIMER_0_INTERR |= TIMER_0_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, true);
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (!TIMER_1_STARTED()) {
			timer1Ptr = function;
			cli();
			TIMER_1_COMP = 0;
			TIMER_1_WAVEFORM = 0;
			TIMER_1_COUNTER = 0;
			TIMER_1_TARGET = timerTicks;
			TIMER_1_INCR |= TIMER_1_INCREM_ENABLE;
			TIMER_1_SET_SCALAR(scalar);
			TIMER_1_INTERR |= TIMER_1_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, true);
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (!TIMER_2_STARTED()) {
			timer2Ptr = function;
			cli();
			TIMER_2_COMP = 0;
			TIMER_2_WAVEFORM = 0;
			TIMER_2_COUNTER = 0;
			TIMER_2_TARGET = timerTicks;
			TIMER_2_INCR |= TIMER_2_INCREM_ENABLE;
			TIMER_2_SET_SCALAR(scalar);
			TIMER_2_INTERR |= TIMER_2_INTERR_ENABLE;
			sei();
			setTimerStarted(timer, true);
			return true;
		}
	}

	return false;
}

#endif