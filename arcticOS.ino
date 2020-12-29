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
 * Made for the Arduino IDE to keep compatibility with Arduino/Teensy boards.
 * 
 * Comes with drivers for the Arduino Uno and SIM5320* so the OS can be tested
 * without needing the dev hardware (which isn't finished yet)
 * 
 * Although it is *technically* C++, I don't really use C++ specific features.
 * 
 * TODO:
 * - Make the OS less hardware-specific (it won't work if you don't have cellular, stuff like that
 */
 
/*
 * KERNEL DEFINES
 */
#define KERNEL_ENABLE_PREEMPTION 1 // This should always be enabled, but it's not required for basic stuff
#define KERNEL_TASK_TICK_TIME 10 // Millisecond count between preemptions.
#define KERNEL_TASK_MAGIC_NUMBER 0x69 // Magic number to determine valid/invalid task.
#define KERNEL_TASK_COUNT 16 // Max tasks. Minimal OS = minimal amount of tasks present, so I'm sticking with 16 (because it's a nice, small number)
#define KERNEL_HANG_ON_TASK_INSERT_FAIL 1 // Should we crash if we fail to insert a task?

/*
 * GLOBAL VARIABLES, FUNCTION DECLARATIONS, AND TYPEDEFS
 */
// Typedefs
typedef struct {
	int magic; // This decides whether or not a task is valid.
	void (*location)(void); // Task function location
	int repeat; // This decides whether or not a task repeats.
	int tick_count; // Used for repeating tasks to define interval.
} kernel_task;

// Variables
int kernel_ticks_elapsed = 0; // How many ticks have elapsed during this second?
int kernel_in_preemption = 0; // Are we currently running preemption?
int kernel_preemption_enabled = 0; // Is preemption even enabled?
kernel_task kernel_tasks[KERNEL_TASK_COUNT]; // The list of all tasks the kernel has

// Functions
void kernel_add_task(kernel_task task); // Add an existing task definition to the task list
void kernel_create_task(void* location); // Create a new, non-repeating task definition and add it to the task list
void kernel_create_repeating_task(void* location, int interval); // Create a new, repeating task definition and add it to the task list
void kernel_preemption(); // Run through all the tasks and do basic timekeeping
void kernel_halt(); // Freeze the whole OS, requiring a hard reset. Only for the most serious of failures!
void kernel_init(); // Initialise the preemption system.

/*
 * IMPORT GLOBAL STRINGS
 */
#include "arcticOS_strings.h"

/*
 * LIBRARIES
 */
#include "betterstr.h"

/*
 * DEVICE DRIVERS
 */

// Hardware-specific
#include "driver_arduino.h"
//#include "driver_i2cFRAM.h"

// Non-specific
//#include "driver_fs.h"

#ifndef DRIVER_BOARD
	#error "A board driver is required."
#endif

#ifndef BOARD_HAS_TIMED_IRQ
	#warning "Board doesn't support timed IRQs! Multiprocessing won't work."
#endif

/*
 * KERNEL
 */

// KERNEL CORE FUNCTIONS
void kernel_init() {
	/* 
	 * Multitasking in modern preemptive systems works by having something called a timed inturrupt.
	 * Basically, the CPU will stop what it is doing after a certain time has passed and run code you specify.
	 * In most systems, this does some advanced task-switching stuff, but we don't need that since all we need
	 * is to make sure that some important things run when they're supposed to. In the case of arcticOS, all we
	 * do is every (configurable) handful of milliseconds we quickly run through our entire list of important 
	 * tasks, disable the ones that need to be disabled, and do some basic timekeeping.
	 */
	 
	if(KERNEL_ENABLE_PREEMPTION) { // We need to make sure that we actually *want* preemption to be enabled by default in this build.
		kernel_preemption_enabled = 1; // Tell the system we are allowed to run preemption
		
		// This sets up a timed inturrupt. This is different across boards and CPUs, so this function is defined in the board driver.
		#ifdef BOARD_HAS_TIMED_IRQ 
			set_timed_irq(kernel_preemption, KERNEL_TASK_TICK_TIME);
		#endif
	}
}

void kernel_halt() {
	kernel_preemption_enabled = 0; // Disable preemption so we don't keep doing tasks.
	debug_print("System halted!\n");
	
	// Go into an endless loop of nothing.
	while(1) {}
}

// MULTIPROCESSING
void kernel_add_task(kernel_task task) {
	// Make sure task is valid by checking to make sure the magic number is valid.
	// If we try to init an invalid task, something has likely gone wrong somewhere.
	// In this case, we just hang the whole OS to make sure nothing bad happens.
	if(task.magic != KERNEL_TASK_MAGIC_NUMBER) kernel_halt();
	
	// Loop through all of our tasks and see if there's an empty space.
	for(int i = 0; i < KERNEL_TASK_COUNT; i ++) {
		if(kernel_tasks[i].magic != KERNEL_TASK_MAGIC_NUMBER) {
			kernel_tasks[i] = task;
			return;
		}
	}
	
	// In some cases, we may want to hang if we fail to add the task to our list.
	// This is fully configurable, but this issue really shouldn't be run into because
	// of how simple arcticOS is. It's enabled by default, because if we're trying to
	// add too many tasks, we've probably found some sort of bug.
	if(KERNEL_HANG_ON_TASK_INSERT_FAIL)
		kernel_halt();
}

void kernel_create_task(void* location) {
	// Create an empty task and fill it with what we need
	kernel_task task;
	task.magic = KERNEL_TASK_MAGIC_NUMBER;
	task.location = location;
	task.repeat = 0;
	task.tick_count = 0;
	kernel_add_task(task); // Add the task to the list
}

void kernel_create_repeating_task(void* location, int interval) {
	// Create an empty task and fill it with what we need
	kernel_task task;
	task.magic = KERNEL_TASK_MAGIC_NUMBER;
	task.location = location;
	task.repeat = 1;
	task.tick_count = interval;
	kernel_add_task(task); // Add the task to the list
}

void kernel_preemption() {
	// Make sure we're not already running preemptiveness
	if(kernel_in_preemption)
		return;
		
	// Tell the system we're in the process of running tasks
	kernel_in_preemption = 1;
		
	// Run through all tasks
	for(int i = 0; i < KERNEL_TASK_COUNT; i ++) {
		if(kernel_tasks[i].magic == KERNEL_TASK_MAGIC_NUMBER) { // Make sure task is valid
			if(kernel_tasks[i].repeat) { // Repeating task?
				if(kernel_ticks_elapsed % kernel_tasks[i].tick_count == 0) {
					kernel_tasks[i].location();
				}
			} else { // No, single run task
				kernel_tasks[i].location();
				kernel_tasks[i].magic = 0x00; // Disable task
			}
		}
	}
	
	kernel_ticks_elapsed ++;
	
	// Reset tick counter if needed
	if(kernel_ticks_elapsed >= 1000 / KERNEL_TASK_TICK_TIME)
		kernel_ticks_elapsed = 0;
	
	// Tell system we're done
	kernel_in_preemption = 0;
	return;
}

void setup() {	
	debug_init(9600);
	debug_print("Board: ");
	debug_print(DRIVER_BOARD_NAME);
	
	#ifdef DRIVER_SCREEN 
		screen_init();
	#endif
	
	#ifdef DRIVER_CELLULAR
		debug_print("Cell: ");
		debug_print(DRIVER_CELLULAR_NAME);
		
		debug_print("Cell handshake ");
		cellular_init();
		if(cellular_handshake()){
			debug_print("OK\n");
			// TODO: Get service and stuff
		} else
			debug_print("FAIL\n");
	#endif
	
	#ifdef DRIVER_FLASH
		debug_print("Flash: ");
		debug_print(DRIVER_FLASH_NAME);
	#endif
		
	#ifdef BOARD_HAS_KEYPAD
		//debug_print("Button Setup\n");
		matrix_init(4, 5, 6, 7, 8, 9, 10, 11); // Initialise the matrix with pins 4-11
		kernel_create_repeating_task(&matrix_refresh_pressed_buttons, 10); // Register a task to refresh those buttons
	#endif
	
	debug_print("Welcome to arcticOS!\n");
	kernel_init();
}

void loop() {
	// I don't have all of the parts I need to test the UI and input, so we'll just use the debug interface for now :)
	
	#ifdef BOARD_HAS_KEYPAD
		for(int x = 0; x < 3; x++) {
			for(int y = 0; y < 5; y++) {
				if(buttons_pressed[x][y] == 1) {
					Serial.print(x);
					Serial.print(",");
					Serial.println(y);
				}
			}
		}
	#endif
}
