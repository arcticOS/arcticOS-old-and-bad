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
 * Main OS file
 * For the Arduino IDE
 * 
 * Previously, I wanted the OS to be modular and portable, for use with 
 * any sort of device. After thinking about it, I've decided that I want
 * this to be made specifically for mobile devices - none of this "iT
 * rUnS oN eVeRyThInG gUyS!!!1!!!oneone!!111!" BS. 
 * 
 * Don't get me wrong, I want people to use this in their own devices. 
 * Whether they just pull the multitasking engine for use in some random
 * other project, or actually run the OS on their phones. But so far,
 * trying to make it an all-round OS is just making it messier and quite
 * honestly, shittier.
 * 
 * So yeah, no more "do this but ONLY IF we have a screen" because any
 * devices that run this OS should realistically have a screen. And a
 * keypad.
 */

/*
 * BUILD SETTINGS
 */
 
#define KERNEL_TASK_COUNT 16
#define KERNEL_ENABLE_WATCHDOG // Print a debug message if multitasking takes too long.
 
/*
 * DRIVERS AND LIBRARIES
 */
 
#include "drivers/polar/board.h"

/*
 * KERNEL STUFF
 */

class KernelTask {
	public:
		void (*location)(void);
		int interval;
		
		KernelTask(void* function_location, int function_interval) {
			location = function_location;
			interval = function_interval;
		}
};

int kernel_in_preemption = 0;
int kernel_allow_preemption = 0;
int kernel_ticks_elapsed = 0;

KernelTask* kernel_tasks[KERNEL_TASK_COUNT];

void kernel_halt() {
	kernel_allow_preemption = 0;
	while(true) {}
}

void kernel_panic(int code) {
	board_screen_rect(0, 0, 8*6, 16);
	board_screen_position(0, 0);
	board_screen_text(1, "PANIC!");
	board_screen_invert_text();
	board_screen_text(1, "PANIC!");
	kernel_halt();
}

void kernel_add_task(void* location, int interval) {
	KernelTask* task = new KernelTask(location, interval);
	for(int i = 0; i < KERNEL_TASK_COUNT; i++) {
		if(kernel_tasks[i] == 0) {
			kernel_tasks[i] = task;
			return;
		}
	}
	
	board_debug_print("BUG: Failed to insert task\n");
	return;
}

void kernel_remove_task(int position) {
	delete kernel_tasks[position];
	kernel_tasks[position] = 0;
}

void kernel_multitask() {
	if(!kernel_in_preemption) {
		kernel_in_preemption = 1;
		
		for(int i = 0; i < KERNEL_TASK_COUNT; i++) {
			// TODO: Single-fire tasks, set task interval, advanced watchdog
			
			if(kernel_tasks[i] != 0) {
				kernel_tasks[i]->location();
			}
		}
	}
	#ifdef KERNEL_ENABLE_WATCHDOG
		else {
			if(kernel_in_preemption > 10) {
				board_debug_print("Watchdog BARK!\n");
			}
			
			kernel_in_preemption ++;
		}
	#endif
	
	kernel_in_preemption = 0;
	return;
}

/*
 * OS STUFF
 */
 
// Wait until all keys released
void os_wait_for_no_keys() {
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
 
// This is where we initialise the board and the kernel.
void setup() {	
	board_init();

	board_set_timed_irq(10, kernel_multitask);

	board_screen_init();
	board_screen_clear();
	board_init_keypad();
	
	kernel_add_task(board_keypad_refresh, 0);
	board_debug_print("Keypad test:\n");
}

// UI here

/*
 * Screen IDs:
 * 0 - clock
 * 1 - dialer
 * 2 - menu
 */
int ui_current_screen = 0;

void ui_switch_screen(int screen) {
	ui_current_screen = screen;
	board_screen_clear();
	board_screen_color(1);
}

const char* ui_menu_time = "12:00";
const char* ui_menu_date = "17/01/21";

const char* ui_dialer_number = "";

void loop() {
	if(ui_current_screen == 0) {
		// Here we just need to display the time, date, and the word "menu"
		// Eventually I'd like to have a seven-segment time font.
		
		board_screen_text(4, ui_menu_time);
		board_screen_text(2, ui_menu_date);
		
		int done = 0;
		while(!done) {
			char c = os_wait_for_key();
			if(c >= '0' && c <= '9') {
				// TODO: Send number pressed to dialer
				ui_switch_screen(1);
				done = 1;
			}
		}
		
	} else if(ui_current_screen == 1) {
		// Here we need to get and display a phone number, as well as offer back and call options
	} else if(ui_current_screen == 2) {
		// Here we need to show SMS, calculator, calendar, notes, and settings.
	}
}
