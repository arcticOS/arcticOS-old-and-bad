/*
 * arcticOS
 * Copyright 2020 Johnny Stene <jhonnystene@protonmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Arduino board definition
 * 
 * This driver should be used as a skeleton - it has no drivers for
 * screen or input, and so it will not compile unless you're planning
 * to completely rewrite os.ino
 * 
 * TODO: 
 * 	- i2c
 *  - SPI(?)
 */

#ifndef BOARD
	#define BOARD

	#define TIMER_INTERRUPT_DEBUG 0
	#define USE_TIMER_1 false
	#define USE_TIMER_2 true
	#define USE_TIMER_3 false
	#define USE_TIMER_4 false
	#define USE_TIMER_5 false
	
	#include <TimerInterrupt.h>
	
	void (*board_irq_handler)(void);
	
	void board_init() {
		Serial.begin(9600);
	}
	
	/*
	 * DEBUG
	 */
	void board_debug_print(const char* value) {
		Serial.print(value);
	}
	
	void board_debug_print(int value) {
		Serial.print(value);
	}
	
	void board_debug_print(char value) {
		Serial.print(value);
	}
	
	/*
	 * IRQ
	 */
	void board_set_timed_irq(int tick_length, void* location) {
		ITimer2.init();
		board_irq_handler = location;
		ITimer2.attachInterruptInterval(tick_length, board_irq_handler);
	}
	
	/*
	 * GPIO
	 */
	#define BOARD_HAS_GPIO
	#define BOARD_HAS_GPIO_PULLUP
	void board_init_gpio(int port, int state) {
		if(state == 0) { // Output
			pinMode(port, OUTPUT);
		} else if(state == 1) { // Input
			pinMode(port, INPUT);
		} else if(state == 2) { // Input Pullup
			pinMode(port, INPUT_PULLUP);
		}
	}
	
	void board_gpio_out(int port, int state) {
		digitalWrite(port, state);
	}
	
	int board_gpio_in(int port) {
		return digitalRead(port);
	}
#endif
