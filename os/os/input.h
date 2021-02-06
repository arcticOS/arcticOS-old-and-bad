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
 * Input handling for arcticOS
 */

#ifndef INPUT_H
	#define INPUT_H
	// Wait until all keys released
	void os_wait_for_no_keys() {
	  board_debug_print("Waiting for no keys to be held...\n");
		while(1) {
			int sum = 0;
			for(int i = 0; i < BOARD_KEYPAD_KEY_COUNT; i++) {
				sum += board_keypad_pressed[i];
			}

			if(sum == 0) return;
		}
	}

	// Wait until a key is pressed and return the keycode
	char os_wait_for_key() {
		board_debug_print("Waiting for key...\n");
		char key = 0; // Returned key

		while(key == 0) {
			for(int i = 0; i < BOARD_KEYPAD_KEY_COUNT; i++) {
				if(board_keypad_pressed[i] == 10) {
					os_wait_for_no_keys();
					key = board_keypad_chars[i];
				}
			}
		}
		return key;
	}

	// Get key pressed
	char os_get_key() {
		for(int i = 0; i < BOARD_KEYPAD_KEY_COUNT; i++) {
			if(board_keypad_pressed[i] > 0) {
				return board_keypad_chars[i];
			}
		}

		return 0; // No key
	}
#endif
