/*
	hard_timer.h - timer definitions for each board
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

#ifndef HARD_TIMER_H
#define HARD_TIMER_H

#include "compile_flags/compile_flags.h"

// available timers to use
#if NUM_TIMERS > 0
	#define HARD_TIMER_ID_OSC // oscilloscope data collection enable
#endif
#if NUM_TIMERS > 1
	#define HARD_TIMER_ID_LED // status LED enable
#endif

typedef enum { // hardware timer type
	HARD_TIMER_INVALID = -1, // invalid counter
	#if NUM_TIMERS >= 1
		HARD_TIMER0,
	#endif
	#if NUM_TIMERS >= 2
		HARD_TIMER1,
	#endif
	#if NUM_TIMERS >= 3
		HARD_TIMER2,
	#endif
	#if NUM_TIMERS >= 4
		HARD_TIMER3,
	#endif
	#if NUM_TIMERS >= 5
		HARD_TIMER4,
	#endif
	#if NUM_TIMERS >= 6
		HARD_TIMER5,
	#endif
	#if NUM_TIMERS >= 7
		HARD_TIMER6,
	#endif
	#if NUM_TIMERS >= 8
		HARD_TIMER7,
	#endif
	#if NUM_TIMERS >= 9
		HARD_TIMER8,
	#endif
	#if NUM_TIMERS >= 10
		HARD_TIMER9,
	#endif
	#if NUM_TIMERS >= 11
		HARD_TIMER10,
	#endif
	#if NUM_TIMERS >= 12
		HARD_TIMER11,
	#endif
	#if NUM_TIMERS >= 13
		HARD_TIMER12,
	#endif
	#if NUM_TIMERS >= 14
		HARD_TIMER13,
	#endif
	#if NUM_TIMERS >= 15
		HARD_TIMER14,
	#endif
	#if NUM_TIMERS >= 16
		HARD_TIMER15,
	#endif
} hard_timer_t; // hardware timer type

// codes when getting hard timer stats
enum HardTimerStatusReturn {
	HARD_TIMER_OK, // hard timer stats retrieved
	HARD_TIMER_SLIGHTLY_OFF, // retrieved values that aren't completely accurate
	HARD_TIMER_FAIL, // failed to get timer values
};

typedef uint32_t freq_t; // hard timer frequency variable
typedef uint8_t timer_priority_t; // hard timer execute priority variable
typedef hard_timer_return_t (*hard_timer_function_ptr_t) (hard_timer_param_t); // timer callback function pointer

#define DEFAULT_HARD_TIMER_PRIORITY 0 // default hard timer priority

// hardware timer priority for claiming timers
struct hardTimerPriority {
	bool slowestTimer: 1; // whether to use slowest timer or not
	bool mostAccurateTimer: 1; // whether to use most accurate timer or not
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Claims a timer based on functional priorities
 * 
 * @param priority priority of timer to get
 * 
 * @return timer claimed
 */
hard_timer_t claimTimer(struct hardTimerPriority *priority);

/**
 * Releases claim on a timer
 * 
 * @param timer timer to release
 * 
 * @return if unclaim was successful
 */
bool unclaimTimer(hard_timer_t timer);

/**
 * Tests if timer is claimed already or not
 * 
 * @param timer timer to check
 * 
 * @return if timer is claimed or not
 */
bool hardTimerClaimed(hard_timer_t timer);

/**
 * Stops hardware timer from executing
 * 
 * @param timer timer to stop
 * 
 * @return if timer was successfully canceled
 */
bool cancelHardTimer(hard_timer_t timer);

/**
 * Starts hardware timer execution
 * 
 * @param timer pointer to timer to start
 * @param freq pointer to desired frequency in Hz
 * @param function pointer to function to call back
 * @param priority priority to run timer at
 * 
 * setHardTimer(HARD_TIMER_INVALID, ...):
 * 
 *                Claimed:     Unclaimed:
 * 
 * Started:           -,           -
 * 
 * Not Started:       -,       Best Timer
 * 
 * setHardTimer(HARD_TIMER#, ...):
 * 
 *                Claimed:     Unclaimed:
 * 
 * Started:         Fail,      Best Timer
 * 
 * Not Started:  HARD_TIMER#,  HARD_TIMER#
 * 
 * @return if timer was successfully set
 */
bool setHardTimer(hard_timer_t *timer, freq_t *freq, hard_timer_function_ptr_t function, timer_priority_t priority);

/**
 * Gets if selected timer was started
 * 
 * @param timer timer to check
 * 
 * @return if timer was started
 */
bool hardTimerStarted(hard_timer_t timer);

#ifdef __cplusplus
}
#endif

#endif