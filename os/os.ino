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
#include "os/time.h"
#include "os/debug.h"

/*
 * CORE STUFF
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
	board_screen_clear();
	board_screen_text(10, 10, 1, 0, "PANIC!");
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

	debug_printf("BUG: Failed to insert task\n");
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
				debug_printf("Watchdog BARK!\n");
			}

			kernel_in_preemption ++;
		}
	#endif

	kernel_in_preemption = 0;
	return;
}

// This is where all system tasks should go, in order of time-sensitivity.
void kernel_tick() {
	time_tick(); // This should always be first
	kernel_multitask();
}

/*
 * OS STUFF
 */

#include "os/input.h" 
#include "os/ui.h"

// Bootup
void setup() {
	board_init();

	board_set_timed_irq(10, kernel_tick);

	board_screen_init();
	board_init_keypad();

	kernel_add_task(board_keypad_refresh, 0);
}

void loop() {
	ui_do_loop();
}

