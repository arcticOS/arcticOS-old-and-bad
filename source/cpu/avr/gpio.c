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

#ifdef __AVR__

#include <kernel/crash.h>

#include <avr/io.h>
#include <cpu/avr/gpio.h>

void AVR_set_pin_as_output(int port, int pin) {
    // Don't try to write where we shouldn't
    if(pin > 7 | pin < 0) OS_crash();
    if(port > 3 | port < 0) OS_crash();

    switch(port) {
        case(0):
            DDRA |= 1 << pin; break;
        case(1):
            DDRB |= 1 << pin; break;
        case(2):
            DDRC |= 1 << pin; break;
        case(3):
            DDRD |= 1 << pin; break;
    }
}

void AVR_set_pin_as_input(int port, int pin) {
    // Don't try to write where we shouldn't
    if(pin > 7 | pin < 0) OS_crash();
    if(port > 3 | port < 0) OS_crash();

    switch(port) {
        case(0):
            DDRA &= ~(1 << pin); break;
        case(1):
            DDRB &= ~(1 << pin); break;
        case(2):
            DDRC &= ~(1 << pin); break;
        case(3):
            DDRD &= ~(1 << pin); break;
    }
}

void AVR_set_pin_low(int port, int pin) {
    // Don't try to write where we shouldn't
    if(pin > 7 | pin < 0) OS_crash();
    if(port > 3 | port < 0) OS_crash();

    switch(port) {
        case(0):
            PORTA &= ~(1 << pin); break;
        case(1):
            PORTB &= ~(1 << pin); break;
        case(2):
            PORTC &= ~(1 << pin); break;
        case(3):
            PORTD &= ~(1 << pin); break;
    }
}

void AVR_set_pin_high(int port, int pin) {
    // Don't try to write where we shouldn't
    if(pin > 7 | pin < 0) OS_crash();
    if(port > 3 | port < 0) OS_crash();

    switch(port) {
        case(0):
            PORTA |= 1 << pin; break;
        case(1):
            PORTB |= 1 << pin; break;
        case(2):
            PORTC |= 1 << pin; break;
        case(3):
            PORTD |= 1 << pin; break;
    }
}

int AVR_read_pin(int port, int pin) {
    // Don't try to read where we shouldn't
    if(pin > 7 | pin < 0) OS_crash();
    if(port > 3 | port < 0) OS_crash();

    switch(port) {
        case(0):
            return (PINA & (1 << pin)) >> pin;
        case(1):
            return (PINB & (1 << pin)) >> pin;
        case(2):
            return (PINC & (1 << pin)) >> pin;
        case(3):
            return (PIND & (1 << pin)) >> pin;
    }
    return 0;
}

#endif