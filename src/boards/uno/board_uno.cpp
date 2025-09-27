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

#include <Arduino.h>

bool initHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar) {
	return true;
}

bool deconstructHardTimer(hardware_timer_t timer) {
	return true;
}

bool cancelHardTimer(hardware_timer_t timer) {
	if (timer == HARD_TIMER0) {
		cli();
		TCCR0B &= ~(0b00000000 | ((1 << CS00) | (1 << CS01) | (1 << CS02)));
		TIMSK0 &= ~(0b00000000 | (1 << OCIE0A));
		sei();
		return true;
	}
	else if (timer == HARD_TIMER1) {
		cli();
		TCCR1B &= ~(0b00000000 | ((1 << CS10) | (1 << CS11) | (1 << CS12)));
		TIMSK1 &= ~(0b00000000 | (1 << OCIE1A));
		sei();
		return true;
	}
	else if (timer == HARD_TIMER2) {
		cli();
		TCCR2B &= ~(0b00000000 | ((1 << CS20) | (1 << CS21) | (1 << CS22)));
		TIMSK2 &= ~(0b00000000 | (1 << OCIE2A));
		sei();
		return true;
	}
	return false;
}

bool setHardTimer(hardware_timer_t timer, hard_timer_function_ptr_t function, prescalar_t scalar, timertick_t timerTicks) {
	if ((timer == HARD_TIMER0 || timer == HARD_TIMER2) && timerTicks >= UINT8_MAX) {
		timerTicks = UINT8_MAX - 1;
	}
	if ((timer == HARD_TIMER0 || timer == HARD_TIMER1) && (scalar == SCALAR_32 || scalar == SCALAR_128)) {
		return false;
	}

	if (timer == HARD_TIMER0) {
		cli();
		TCCR0A = 0;
		TCCR0B = 0;
		TCNT0 = 0;
		OCR0A = timerTicks;
		TCCR0A |= (1 << WGM01);
		if (scalar == SCALAR_1 || scalar == SCALAR_64 || scalar == SCALAR_1024) {
			TCCR0B |= (1 << CS00);
		}
		if (scalar == SCALAR_8 || scalar == SCALAR_64) {
			TCCR0B |= (1 << CS01);
		}
		if (scalar == SCALAR_256 || scalar == SCALAR_1024) {
			TCCR0B |= (1 << CS02);
		}
		TIMSK0 |= (1 << OCIE0A);
		sei();
		return true;
	}
	else if (timer == HARD_TIMER1) {
		cli();
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = timerTicks;
		TCCR1B |= (1 << WGM12);
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
		sei();
		return true;
	}
	else if (timer == HARD_TIMER2) {
		cli();
		TCCR2A = 0;
		TCCR2B = 0;
		TCNT2  = 0;
		OCR2A = timerTicks;
		TCCR2A |= (1 << WGM21);
		if (scalar == SCALAR_1 || scalar == SCALAR_32 || scalar == SCALAR_128 || scalar == SCALAR_1024) {
			TCCR2B |= (1 << CS20);
		}
		if (scalar == SCALAR_8 || scalar == SCALAR_32 || scalar == SCALAR_256 || scalar == SCALAR_1024) {
			TCCR2B |= (1 << CS21);
		}
		if (scalar == SCALAR_64 || scalar == SCALAR_128 || scalar == SCALAR_256 || scalar == SCALAR_1024) {
			TCCR2B |= (1 << CS22);
		}
		TIMSK2 |= (1 << OCIE2A);
		sei();
		return true;
	}
	return false;
}

#endif