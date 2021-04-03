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
 * This file contains the needed code to setup the CPU on AVR.
 */

#ifdef __AVR__
#include <avr/io.h>
// TODO: Changeme
#define AVR_MOSI 3
#define AVR_MISO 4
#define AVR_SCK 5

void OS_init_cpu() {
    // TODO: Switch to external oscillator if needed

    DDRB = (1<<AVR_MOSI) | (1<<AVR_MISO) | (1<<AVR_SCK);
    
}
#endif