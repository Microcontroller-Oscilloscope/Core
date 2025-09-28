/*
	board_uno.cpp - configuration for Arduino Uno
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
//#include <util/atomic.h>
#include "../../timer.h"

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

#define TIMER_0_INITIALIZED() (TIMER_0_SCAL & TIMER_0_SCALAR_ENABLE) // if timer 0 was initialized
#define TIMER_0_STARTED() (TIMER_0_INTERR & TIMER_0_INTERR_ENABLE) // if timer 0 was started

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

#define TIMER_1_INITIALIZED() (TIMER_1_SCAL & TIMER_1_SCALAR_ENABLE) // if timer 1 was initialized
#define TIMER_1_STARTED() (TIMER_1_INTERR & TIMER_1_INTERR_ENABLE) // if timer 1 was started

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

#define TIMER_2_INITIALIZED() (TIMER_2_SCAL & TIMER_2_SCALAR_ENABLE) // if timer 2 was initialized
#define TIMER_2_STARTED() (TIMER_2_INTERR & TIMER_2_INTERR_ENABLE) // if timer 2 was started

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

/****************************
 * Timer Functions
****************************/

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

bool timerInitialized(hardware_timer_t timer) {

	if (timer == HARD_TIMER0) {
		if (TIMER_0_INITIALIZED()) {
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (TIMER_1_INITIALIZED()) {
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (TIMER_2_INITIALIZED()) {
			return true;
		}
	}

	return false;
}

bool timerStarted(hardware_timer_t timer) {
	
	if (timer == HARD_TIMER0) {
		if (TIMER_0_STARTED()) {
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (TIMER_1_STARTED()) {
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (TIMER_2_STARTED()) {
			return true;
		}
	}

	return false;
}

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {

	if (SCALAR_OUT_OF_BOUNDS(timer, scalar)) {
		return false;
	}
	
	if (timer == HARD_TIMER0) {
		if (!TIMER_0_INITIALIZED()) {
			cli();
			TIMER_0_SET_SCALAR(scalar);
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (!TIMER_1_INITIALIZED()) {
			cli();
			TIMER_1_SET_SCALAR(scalar);
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (!TIMER_2_INITIALIZED()) {
			cli();
			TIMER_2_SET_SCALAR(scalar);
			sei();
			return true;
		}
	}
	
	return false;
}

bool deconstructHardTimer(hardware_timer_t timer) {

	if (timer == HARD_TIMER0) {
		if (TIMER_0_INITIALIZED()) {
			cli();
			TIMER_0_SCAL &= ~TIMER_0_SCALAR_ENABLE;
			TIMER_0_INTERR &= ~TIMER_0_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (TIMER_1_INITIALIZED()) {
			cli();
			TIMER_1_SCAL &= ~TIMER_1_SCALAR_ENABLE;
			TIMER_1_INTERR &= ~TIMER_1_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (TIMER_2_INITIALIZED()) {
			cli();
			TIMER_2_SCAL &= ~TIMER_2_SCALAR_ENABLE;
			TIMER_2_INTERR &= ~TIMER_2_INTERR_ENABLE;
			sei();
			return true;
		}
	}

	return true;
}

bool cancelHardTimer(hardware_timer_t timer) {
	if (timer == HARD_TIMER0) {
		if (TIMER_0_STARTED()) {
			cli();
			TIMER_0_INTERR &= ~TIMER_0_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (TIMER_1_STARTED()) {
			cli();
			TIMER_1_INTERR &= ~TIMER_1_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (TIMER_2_STARTED()) {
			cli();
			TIMER_2_INTERR &= ~TIMER_2_INTERR_ENABLE;
			sei();
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
			cli();
			TIMER_0_COMP = 0;
			TIMER_0_WAVEFORM = 0;
			TIMER_0_COUNTER = 0;
			TIMER_0_TARGET = timerTicks;
			TIMER_0_INCR |= TIMER_0_INCREM_ENABLE;
			TIMER_0_SET_SCALAR(scalar);
			TIMER_0_INTERR |= TIMER_0_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER1) {
		if (!TIMER_1_STARTED()) {
			cli();
			TIMER_1_COMP = 0;
			TIMER_1_WAVEFORM = 0;
			TIMER_1_COUNTER = 0;
			TIMER_1_TARGET = timerTicks;
			TIMER_1_INCR |= TIMER_1_INCREM_ENABLE;
			TIMER_1_SET_SCALAR(scalar);
			TIMER_1_INTERR |= TIMER_1_INTERR_ENABLE;
			sei();
			return true;
		}
	}
	else if (timer == HARD_TIMER2) {
		if (!TIMER_2_STARTED()) {
			cli();
			TIMER_2_COMP = 0;
			TIMER_2_WAVEFORM = 0;
			TIMER_2_COUNTER = 0;
			TIMER_2_TARGET = timerTicks;
			TIMER_2_INCR |= TIMER_2_INCREM_ENABLE;
			TIMER_2_SET_SCALAR(scalar);
			TIMER_2_INTERR |= TIMER_2_INTERR_ENABLE;
			sei();
			return true;
		}
	}

	return false;
}

#endif