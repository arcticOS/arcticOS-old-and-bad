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
 * This file includes the needed files to run an ATMega2560.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define OS_TASK_STACK_SIZE 64

#define CPU_HAS_SECURE_SALT_STORAGE

#define AVR_HAS_EXTERNAL_CLOCK
// TODO: Finish SPI (http://www.avrbeginners.net/architecture/spi/spi.html | https://avrgeeks.com/spi-of-avr/ | www.tinkerer.eu/AVRLib/SPI/)
// TODO: Pin I/O
// TODO: Serial I/O (https://appelsiini.net/2011/simple-usart-with-avr-libc/)
// TODO: I2C
// TODO: EEPROM (https://www.exploreembedded.com/wiki/Permanent_Data_Storage_with_AVR_Internal_EEPROM)