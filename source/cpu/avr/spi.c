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
 * This file contains the needed code for using SPI on an AVR CPU.
 */

#ifdef __AVR__
#include <avr/io.h>
void OS_SPI_send_data(const char* data, int length) {
    // Declare this CPU as a master
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL); // TODO: Do we really need to do this?
    
    // Send data
    for(int i = 0; i < length; i++) {
        SPDR = data[i];
        while((SPSR & (1<<SPIF)) == 0) {}
    }
}

char OS_SPI_read_byte() {
    while((SPSR & (1<<SPIF)) == 0) {}
    return SPDR;
}
#endif