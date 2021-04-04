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
 * 
 * PLEASE NOTE! Some of this code is released under a separate license!
 * It is available at "licenses/MULTITASKING_LICENSE" 
 * 
 * ALSO PLEASE NOTE! This probably doesn't work! 
 */

#ifdef __AVR__
#include <kernel/multitasking.h>

void save_context(OS_TASK* task) {
	asm volatile (	"push	r0						\n\t"
					"in		r0, __SREG__			\n\t"
					"cli							\n\t"
					"push	r0						\n\t"
					"push	r1						\n\t"
					"clr	r1						\n\t"
					"push	r2						\n\t"
					"push	r3						\n\t"
					"push	r4						\n\t"
					"push	r5						\n\t"
					"push	r6						\n\t"
					"push	r7						\n\t"
					"push	r8						\n\t"
					"push	r9						\n\t"
					"push	r10						\n\t"
					"push	r11						\n\t"
					"push	r12						\n\t"
					"push	r13						\n\t"
					"push	r14						\n\t"
					"push	r15						\n\t"
					"push	r16						\n\t"
					"push	r17						\n\t"
					"push	r18						\n\t"
					"push	r19						\n\t"
					"push	r20						\n\t"
					"push	r21						\n\t"
					"push	r22						\n\t"
					"push	r23						\n\t"
					"push	r24						\n\t"
					"push	r25						\n\t"
					"push	r26						\n\t"
					"push	r27						\n\t"
					"push	r28						\n\t"
					"push	r29						\n\t"
					"push	r30						\n\t"
					"push	r31						\n\t"
					"mov	r26, %[current_task]	\n\t"
					"mov	r27, %[current_task2]    \n\t"
					"in		r0, 0x3d				\n\t"
					"st		x+, r0					\n\t"
					"in		r0, 0x3e				\n\t"
					"st		x+, r0					\n\t" :: 
                    [current_task] "r" (task),
                    [current_task2] "r" (task + 1));
}

void restore_context(OS_TASK* task) {
	asm volatile (	"mov	r26, %[current_task]		\n\t"
					"mov	r27, %[current_task2]	\n\t"
					"ld		r28, x+					\n\t"
					"out	__SP_L__, r28			\n\t"
					"ld		r29, x+					\n\t"
					"out	__SP_H__, r29			\n\t"
					"pop	r31						\n\t"
					"pop	r30						\n\t"
					"pop	r29						\n\t"
					"pop	r28						\n\t"
					"pop	r27						\n\t"
					"pop	r26						\n\t"
					"pop	r25						\n\t"
					"pop	r24						\n\t"
					"pop	r23						\n\t"
					"pop	r22						\n\t"
					"pop	r21						\n\t"
					"pop	r20						\n\t"
					"pop	r19						\n\t"
					"pop	r18						\n\t"
					"pop	r17						\n\t"
					"pop	r16						\n\t"
					"pop	r15						\n\t"
					"pop	r14						\n\t"
					"pop	r13						\n\t"
					"pop	r12						\n\t"
					"pop	r11						\n\t"
					"pop	r10						\n\t"
					"pop	r9						\n\t"
					"pop	r8						\n\t"
					"pop	r7						\n\t"
					"pop	r6						\n\t"
					"pop	r5						\n\t"
					"pop	r4						\n\t"
					"pop	r3						\n\t"
					"pop	r2						\n\t"
					"pop	r1						\n\t"
					"pop	r0						\n\t"
					"out	__SREG__, r0			\n\t"
					"pop	r0						\n\t" :: 
                    [current_task] "r" (task),
                    [current_task2] "r" (task + 1));
}

void OS_do_context_switch() {
    save_context(&OS_tasks[OS_current_task]);
    OS_current_task ++;
    if(OS_current_task > 2) OS_current_task = 0;
    restore_context(&OS_tasks[OS_current_task]);
}

#endif