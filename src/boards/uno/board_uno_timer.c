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

typedef enum {
	SCALAR_1, // timer prescalar of 1, timers 0-2
	SCALAR_8, // timer prescalar of 8, timers 0-2
	SCALAR_32, // timer prescalar of 32, timer 2
	SCALAR_64, // timer prescalar of 64, timers 0-2
	SCALAR_128, // timer prescalar of 128, timer 2
	SCALAR_256, // timer prescalar of 256, timers 0-2
	SCALAR_1024, // timer prescalar of 1024, timers 0-2
} prescalar_t; // pre scalar type
typedef uint16_t timertick_t; // timer tick type

uint8_t timerStates = 0U; // started states and claimed states

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
void setTimerStarted(hard_timer_t timer, bool state) {

	if (timer == HARD_TIMER_INVALID) {
		return;
	}
	if (state) {
		timerStates |= (1 << timer);
	}
	else {
		timerStates &= (~(1 << timer));
	}
}

/**
 * Sets timer claimed state
 * 
 * @param timer timer to set
 * @param state whether or not timer is claimed
 */
void setTimerClaimed(hard_timer_t timer, bool state) {

	if (timer == HARD_TIMER_INVALID) {
		return;
	}
	if (state) {
		timerStates |= (1 << (timer + 3));
	}
	else {
		timerStates &= (~(1 << (timer + 3)));
	}
}

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
void getStats(freq_t *freq, hard_timer_t timer, prescalar_t *scalar, timertick_t *timerTicks) {

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

bool hardTimerClaimed(hard_timer_t timer) {

	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	return (!!((1 << (3 + timer)) & timerStates));
}

/**
 * Tests if given timer is available to claim
 * 
 * @param timer timer to test
 * 
 * @return if timer is available
 */
bool availableClaim(hard_timer_t timer) {
	if (!hardTimerClaimed(timer) && !hardTimerStarted(timer)) {
		setTimerClaimed(timer, true);
		return true;
	}
	return false;
}

hard_timer_t claimTimer(struct hardTimerPriority *priority) {

	// checks priorities
	if (priority -> slowestTimer) {
		if (availableClaim(HARD_TIMER1)) {
			return HARD_TIMER1;
		}
	}
	if (priority -> mostAccurateTimer) {
		if (availableClaim(HARD_TIMER2)) {
			return HARD_TIMER2;
		}
	}

	// uses default order if no priority matched
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (availableClaim(i)) {
			return i;
		}
	}

	return HARD_TIMER_INVALID;
}

bool unclaimTimer(hard_timer_t timer) {
	if (hardTimerClaimed(timer)) {
		setTimerClaimed(timer, false);
		return true;
	}
	return false;
}

/**
 * Gets hard timer stats for target frequency
 * 
 * @param freq pointer to desired frequency in Hz
 * @param timer pointer to timer ID
 * @param scalar pointer to scalar value
 * @param timerTicks pointer to desired tick count
 * 
 * @return result of getting timer stats
 * 
 * @note freq value is changed to actual freq if values are slightly off
 */
enum HardTimerStatusReturn getHardTimerStats(freq_t *freq, hard_timer_t *timer, prescalar_t *scalar, timertick_t *timerTicks) {

	if (hardTimerStarted(*timer)) {
		return HARD_TIMER_FAIL;
	}
	if (hardTimerClaimed(*timer)) {

		getStats(freq, *timer, scalar, timerTicks);

		if (sameFreq(*freq, *scalar, *timerTicks)) {
			return HARD_TIMER_OK;
		}
		else {
			return HARD_TIMER_SLIGHTLY_OFF;
		}
	}

	if (*freq < FREQ_MIN_8_COUNTER && (*timer == HARD_TIMER1 || *timer == HARD_TIMER_INVALID)) {
		// calculates slow frequencies for timer 1

		if (hardTimerStarted(HARD_TIMER1) || (*timer == HARD_TIMER_INVALID && hardTimerClaimed(HARD_TIMER1))) {
			// slow timer unavailable
			return HARD_TIMER_FAIL;
		}

		*timer = HARD_TIMER1;
		getStats(freq, *timer, scalar, timerTicks);

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
			
			*timer = HARD_TIMER0;
			getStats(&tempFreq, *timer, scalar, timerTicks);

			if (sameFreq(tempFreq, *scalar, *timerTicks)) {
				status = HARD_TIMER_OK;
			}
			else {
				status = HARD_TIMER_SLIGHTLY_OFF;
			}
		}

		// gets timer 1
		if (!hardTimerStarted(HARD_TIMER1)) {

			if (*timerTicks != 0) {
				// timer 0 in use

				*timer = HARD_TIMER1;
				getStats(&tempFreq, *timer, scalar, timerTicks);

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

				*timer = HARD_TIMER2;
				getStats(&tempFreq, *timer, scalar, timerTicks);

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

		if (*timer == HARD_TIMER_INVALID) {
			return HARD_TIMER_FAIL;
		}

		return status;
	}
}

bool hardTimerStarted(hard_timer_t timer) {

	if (timer == HARD_TIMER_INVALID) {
		return false;
	}
	return !!((1 << timer) & timerStates);
}

/**
 * Cancels hard timer
 * 
 * @param num timer id
 */
#define CANCEL_HARD_TIMER(num) \
	cli(); \
	CONCATENATE3(TIMER_, num, _SCAL) &= ~CONCATENATE3(TIMER_, num, _SCALAR_ENABLE); \
	CONCATENATE3(TIMER_, num, _INTERR) &= ~CONCATENATE3(TIMER_, num, _INTERR_ENABLE); \
	sei(); \
	CONCATENATE3(timer, num, Ptr) = NULL

bool cancelHardTimer(hard_timer_t timer) {

	if (hardTimerStarted(timer)) {
	if (timer == HARD_TIMER0) {
			CANCEL_HARD_TIMER(0);
	}
	else if (timer == HARD_TIMER1) {
			CANCEL_HARD_TIMER(1);
	}
	else if (timer == HARD_TIMER2) {
			CANCEL_HARD_TIMER(2);
		}
			setTimerStarted(timer, false);
			return true;
	}

	return false;
}

/**
 * Sets hard timer
 * 
 * @param num timer number
 */
#define SET_HARD_TIMER(num, scalar, timerTicks, function) \
	CONCATENATE3(timer, num, Ptr) = function; \
	cli(); \
	CONCATENATE3(TIMER_, num, _COMP) = 0; \
	CONCATENATE3(TIMER_, num, _WAVEFORM) = 0; \
	CONCATENATE3(TIMER_, num, _COUNTER) = 0; \
	CONCATENATE3(TIMER_, num, _TARGET) = timerTicks; \
	CONCATENATE3(TIMER_, num, _INCR) |= CONCATENATE3(TIMER_, num, _INCREM_ENABLE); \
	CONCATENATE3(TIMER_, num, _SET_SCALAR)(scalar); \
	CONCATENATE3(TIMER_, num, _INTERR) |= CONCATENATE3(TIMER_, num, _INTERR_ENABLE); \
	sei()

bool setHardTimer(hard_timer_t *timer, freq_t *freq, hard_timer_function_ptr_t function, timer_priority_t priority) {

	if (function == NULL || freq == NULL || timer == NULL) {
		return false;
	}
	if (*freq == (freq_t)0 || *freq > FREQ_MAX) {
		return false;
	}

	prescalar_t scalar;
	timertick_t timerTicks;

	if (getHardTimerStats(freq, timer, &scalar, &timerTicks) == HARD_TIMER_FAIL) {
		return false;
	}

	if (
		(*timer != HARD_TIMER1 && timerTicks >= UINT8_MAX) || // tests ticks out of bounds
		(*timer != HARD_TIMER2 && (scalar == SCALAR_32 || scalar == SCALAR_128)) // tests scalar out of bounds
	) {
		return false;
	}

	if (!hardTimerStarted(*timer)) {
	if (*timer == HARD_TIMER0) {
			SET_HARD_TIMER(0, scalar, timerTicks, function);
	}
	else if (*timer == HARD_TIMER1) {
			SET_HARD_TIMER(1, scalar, timerTicks, function);
	}
	else if (*timer == HARD_TIMER2) {
			SET_HARD_TIMER(2, scalar, timerTicks, function);
		}

			setTimerStarted(*timer, true);
			return true;
	}

	return false;
}

#endif