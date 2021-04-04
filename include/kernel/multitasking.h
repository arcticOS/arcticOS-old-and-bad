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

#include <cpu/cpu.h>

// If we decide to move to a fully multitasked system in the future, we'll need
// to add more things to this.
typedef struct {
    void* stack_pointer;
} OS_TASK;

OS_TASK OS_tasks[2];
int OS_current_task;

void OS_init_multitasking();
void OS_master_timed_irq();

OS_TASK OS_create_new_task(void* function, void* stack_pointer);

void OS_do_context_switch();