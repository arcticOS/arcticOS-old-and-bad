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
 * Polar devkit board definitions
 * The Polar devkit is quite simple and is designed to be easy to assemble.
 * It consists of an Arduino Mega and a 3x5 button matrix with the following pin assignments:
 * 
 *      23  25  27
 *      --  --  --
 * 
 * 29|
 * 
 * 31|
 * 
 * 33|
 * 
 * 35|
 * 
 * 37|
 * 
 * The Arduino Mega is connected via i2c to an Arduino Nano, which is itself connected to a display.
 * 
 * TODO: 
 * 	- i2c
 * 	- Display code
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
	
	/*
	 * KEYPAD
	 */
	 
	#define BOARD_HAS_KEYPAD
	#define BOARD_KEYPAD_KEY_COUNT 15
	
	int board_keypad_pressed[BOARD_KEYPAD_KEY_COUNT];
	
	char board_keypad_chars[BOARD_KEYPAD_KEY_COUNT] = {'A', 'O', 'E', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
	
	int board_keypad_pins_output[3] = {23, 25, 27};
	int board_keypad_pins_input[5] = {29, 31, 33, 35, 37};
	
	void board_init_keypad() {
		for(int i = 0; i < 3; i ++) {
			board_init_gpio(board_keypad_pins_output[i], 0); // Init pin as output
		}
		
		for(int i = 0; i < 5; i++) {
			board_init_gpio(board_keypad_pins_input[i], 2); // Init pin as pulled up input
		}
	}
	
	void board_keypad_refresh() {
		for(int x = 0; x < 3; x++) {
			board_gpio_out(board_keypad_pins_output[x], 0);
			
			for(int y = 0; y < 5; y++) {
				int state = board_gpio_in(board_keypad_pins_input[y]);
				
				if(state == 0) board_keypad_pressed[(y*3) + x] ++;
				else board_keypad_pressed[(y*3) + x] = 0;
			}
			
			board_gpio_out(board_keypad_pins_output[x], 1);
		}
	}
	
	#define BOARD_HAS_SCREEN
	
	int board_screen_width = 0;
	int board_screen_height = 0;
	
	void board_screen_init() {
		Serial1.begin(9600);
		String inString = Serial1.readStringUntil('\n'); // Wait for screen ready
		
		board_screen_width = inString.substring(1, 4).toInt();
		board_screen_height = inString.substring(5, 8).toInt();
		
		// If we failed to get the size just fuggin default to whatever I have plugged in lol
		if(board_screen_width == 0) board_screen_width = 240;
		if(board_screen_height == 0) board_screen_height = 320;
	}
	
	void board_screen_clear() {
		Serial1.println("C");
		Serial1.readStringUntil('\n');
	}
	
	void board_screen_print_value(int value) {
		if(value < 100) Serial1.print("0");
		if(value < 10) Serial1.print("0");
		Serial1.print(value);
	}
	
	void board_screen_text(int x, int y, int size, int align, const char* data) {
		Serial1.print("T");
		board_screen_print_value(x);
		board_screen_print_value(y);
		Serial1.print(size);
		Serial1.print(align);
		Serial1.println(data);
		Serial1.readStringUntil('\n');
	}
#endif
