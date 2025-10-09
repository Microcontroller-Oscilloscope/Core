/*
	board_pico1w_pins.h - pin configuration for Raspberry Pi Pico W
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

/**
 *                                   +----+
 *                               +---|    |---+
 *     (D0, TX0, SDA0, CIPO0)   1|   +----+   |40  VBUS
 *       (D1, RX0, SCL0, CS0)   2|            |39  VSYS
 *                        GND   3| LED D25    |38  GND
 *           (D2, SDA1, SCK0)   4|            |37  3V3_EN
 *          (D3, SCL1, COPI0)   5|            |36  3V3_OUT
 *     (D4, TX1, SDA0, CIPO0)   6|            |35  AREF
 *       (D5, RX1, SCL0, CS0)   7|            |34  (D28, A2)
 *                        GND   8|            |33  GND, AGND
 *           (D6, SDA1, SCK0)   9|            |32  (D27, A1, SCL1)
 *          (D7, SCL1, COPI0)  10|            |31  (D26, A0, SDA1)
 *     (D8, TX1, SDA0, CIPO1)  11|            |30  RUN
 *       (D9, RX1, SCL0, CS1)  12|            |29  (D22)
 *                        GND  13|            |28  GND
 *          (D10, SDA1, SCK1)  14|            |27  (D21, SCL0)
 *         (D11, SCL1, COPI1)  15|            |26  (D20, SDA0)
 *    (D12, TX0, SDA0, CIPO1)  16|            |25  (D19, SCL1, COPI0)
 *      (D13, RX0, SCL0, CS1)  17|            |24  (D18, SDA1, SCK0)
 *                        GND  18|            |23  GND
 *          (D14, SDA1, SCK1)  19|            |22  (D17, RX0, SCL0, CS0)
 *         (D15, SCL1, COPI1)  20|            |21  (D16, TX0, SDA0, CIPO0)
 *                               +------------+
 * 
 * All digital pins support PWM
 * 		8 PWM generators with 2 channels each
 * 		0A: D0, D16
 * 		0B: D1, D17
 * 		1A: D2, D18
 * 		1B: D3, D19
 * 		2A: D4, D20
 * 		2B: D5, D21
 * 		3A: D6, D22
 * 		3B: D7
 * 		4A: D8
 * 		4B: D9
 * 		5A: D10, D26
 * 		5B: D11, D27
 * 		6A: D12, D28
 * 		6B: D13
 * 		7A: D14
 * 		7B: D15
 */

#ifdef PLATFORMIO
	#include <pins_arduino.h>
#endif

#ifndef NUM_IO_PINS
	#define NUM_IO_PINS 26 // number pins available to controller
#endif

// digital (excluding D25 as its not accessible)
#ifndef D0
	#define D0 0
#endif
#ifndef D1
	#define D1 1
#endif
#ifndef D2
	#define D2 2
#endif
#ifndef D3
	#define D3 3
#endif
#ifndef D4
	#define D4 4
#endif
#ifndef D5
	#define D5 5
#endif
#ifndef D6
	#define D6 6
#endif
#ifndef D7
	#define D7 7
#endif
#ifndef D8
	#define D8 8
#endif
#ifndef D9
	#define D9 9
#endif
#ifndef D10
	#define D10 10
#endif
#ifndef D11
	#define D11 11
#endif
#ifndef D12
	#define D12 12
#endif
#ifndef D13
	#define D13 13
#endif
#ifndef D14
	#define D14 14
#endif
#ifndef D15
	#define D15 15
#endif
#ifndef D16
	#define D16 16
#endif
#ifndef D17
	#define D17 17
#endif
#ifndef D18
	#define D18 18
#endif
#ifndef D19
	#define D19 19
#endif
#ifndef D20
	#define D20 20
#endif
#ifndef D21
	#define D21 21
#endif
#ifndef D22
	#define D22 22
#endif
#ifndef D26
	#define D26 26
#endif
#ifndef D27
	#define D27 27
#endif
#ifndef D28
	#define D28 28
#endif

// serial (serials can be assigned to most pins)
#ifndef SERIAL_COUNT
	#define SERIAL_COUNT 2
#endif

// SPI (SPI can be assigned to most pins)
#ifndef SPI_COUNT
	#define SPI_COUNT 2
#endif

// I2C (I2C can be assigned to most pins)
#ifndef I2C_COUNT
	#define I2C_COUNT 2
#endif

// analog
#ifndef ADC0
	#define ADC0 D26
#endif
#ifndef ADC1
	#define ADC1 D27
#endif
#ifndef ADC2
	#define ADC2 D28
#endif
#ifndef ADC_COUNT
	#define ADC_COUNT 3
#endif

// DAC
#ifndef DAC_COUNT
	#define DAC_COUNT 0
#endif

// PWM
#ifndef PWM_COUNT
	#define PWM_COUNT 16
#endif

// LED
#ifndef INTERNAL_LED
	#define INTERNAL_LED LED_BUILTIN
#endif
#ifndef LED_COUNT
	#define LED_COUNT 1
#endif