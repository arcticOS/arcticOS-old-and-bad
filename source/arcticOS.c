/*
 * arcticOS Version 0.2-alpha
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
#include <cpu/timed_irq.h>

#include <kernel/crash.h>
#include <kernel/multitasking.h>

#include <math/crypto.h>

#include <fs/mat.h>

int main(void) {
    OS_init_cpu();
    OS_register_master_timed_irq(&OS_master_timed_irq);

    // OS loop
    for(;;) {}
}