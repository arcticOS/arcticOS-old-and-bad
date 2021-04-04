/*
 * arcticOS
 * Copyright (c) 2021 Johnny Stene <jhonnystene@protonmail.com>
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
 */

#include <stdint.h>
#include <kernel/multitasking.h>

static uint8_t OS_main_task_stack[OS_TASK_STACK_SIZE];
static uint8_t OS_sys_task_stack[OS_TASK_STACK_SIZE];

static void OS_main_task() {

}

static void OS_sys_task() {

}

OS_TASK OS_create_new_task(void* function, void* stack_pointer) {
    uint8_t* stack = &stack_pointer[OS_TASK_STACK_SIZE - 1];
    stack[0] = (uint16_t) function & 0xFF;
    stack[-1] = (uint16_t) function >> 8;
    for(int i = -2; i > -34; i--) {
        stack[i] = 0;
    }
    stack[-34] = 0x80;

    OS_TASK new_task;
    new_task.stack_pointer = stack - 35;
    return new_task;
}

void OS_init_multitasking() {
    OS_current_task = 0;
    OS_tasks[0] = OS_create_new_task(&OS_main_task, &OS_main_task_stack);
    OS_tasks[1] = OS_create_new_task(&OS_sys_task, &OS_sys_task_stack);
}

void OS_master_timed_irq() {
    /*
     * An important question to ask is whether or not a "real" multitasking system is
     * useful in this scenario. It would help make some things considerably easier,
     * but since we're running on a Harvard architecture, we can't just load in some
     * extra binaries. 
     * 
     * Because of this, the best thing to do is most likely to have 2-process multitasking,
     * where core functions run on one thread and other functions run on the other. Luckily,
     * this can be easily tweaked to become a full multitasking system later since we'll still
     * need context switching.
     */
    OS_do_context_switch();
}