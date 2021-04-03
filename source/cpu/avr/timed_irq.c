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
 * This file contains the needed code to register the "master" timed IRQ on AVR.
 */

#ifdef __AVR__
#include <kernel/multitasking.h>

#include <avr/io.h>
#include <avr/interrupt.h>

void OS_disable_interrupts() {
    cli();
}

void OS_enable_interrupts() {
    sei();
}

void OS_register_master_timed_irq() {
    OS_disable_interrupts();
    #ifdef AVR_HAS_EXTERNAL_CLOCK
        TCCR1B |= 1<<CS11; // Divide clock by 8
        OCR1A = 1250;
    #else
        TCCR1B |= 1<<CS11 | 1<<CS10; // Divide clock by 64
        OCR1A = 2500;
    #endif
    TCCR1B |= 1<<WGM12;
    TIMSK1 |= 1<<OCIE1A;
    OS_enable_interrupts();
}

ISR(TIMER1_COMPA_vect) {
    OS_master_timed_irq();
}
#endif