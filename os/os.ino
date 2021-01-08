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
 * Not using the Arduino IDE? Compile "os.cpp" instead.
 * 
 * This is in the process of being completely rewritten to be smaller, faster, and more modular.
 */

/*
 * BUILD SETTINGS
 */
 
#define KERNEL_TASK_COUNT 16
#define KERNEL_ENABLE_WATCHDOG // Print a debug message if multitasking takes too long.
 
/*
 * DRIVERS AND LIBRARIES
 */
 
#include "drivers/arduino/board.h"

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
			// TODO: Single-fire tasks, set task interval
			
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
 
void preemption_test() {
	board_debug_print("hello\n");
}
 
// This is where we initialise the board and the kernel.
void setup() {	
	board_init();

	board_set_timed_irq(10, kernel_multitask);
	
	kernel_add_task(&preemption_test, 0);
}

void loop() {
	
}
