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
 * Arduino driver
 * Tested with Arduino Uno
 * 
 * This will be a good starting point for the final board driver.
 * It uses an ATMEGA2560-16AU so it is compatible with Arduino libraries,
 * all it'll need is some tweaks to pin assignments and stuff.
 */

#ifndef DRIVER_BOARD
	#define DRIVER_BOARD
	#define DRIVER_BOARD_NAME "Arduino\n"
	
	#define BOARD_HAS_I2C
	#define BOARD_HAS_SERIAL
	#define BOARD_HAS_TIMED_IRQ
	#define BOARD_HAS_KEYPAD
	#define BOARD_HAS_SPI
	
	// TODO: SPI
	#include <SPI.h>
	
	// Arduino provides MSBFISRT, LSBFIRST, SPI_MODEs, etc...
	// Other drivers should provide their own.
	
	void spi_init() {
		SPI.begin();
	}
	
	void spi_start_transaction(int freq, int bit_order, int mode) {
		SPISettings(freq, bit_order, mode);
	}
	
	void spi_send8(uint8_t byte) {
		SPI.transfer(byte);
	}
	
	void spi_send16(uint16_t data) {
		SPI.transfer(data);
	}
	
	void spi_send32(uint32_t data) {
		SPI.transfer(data);
	}
	
	#include <Wire.h>
	void i2c_init() {
		Wire.begin();
	}
	
	void i2c_start(int device) {
		Wire.beginTransmission(4);
	}
	
	void i2c_write_string(const char* string) {
		Wire.write(string);
	}
	
	void i2c_send_byte(int b) {
		Wire.write(b);
	}
	
	void i2c_end() {
		Wire.endTransmission();
	}
	
	void i2c_request(int id, int count) {
		Wire.requestFrom(id, count);
	}
	
	char i2c_get() {
		if(Wire.available()) {
			return Wire.read();
		}
		
		return 0;
	}
	
	void gpio_init(int port, int out) {
		if(out) pinMode(port, OUTPUT);
		else pinMode(port, INPUT);
	}
	
	void gpio_out(int port, int state) {
		digitalWrite(port, state);
	}
	
	int gpio_in(int port) {
		return digitalRead(port);
	}
	
	#define TIMER_INTERRUPT_DEBUG 0
	#define USE_TIMER_1 false
	#define USE_TIMER_2 true
	#define USE_TIMER_3 false
	#define USE_TIMER_4 false
	#define USE_TIMER_5 false
	#include <TimerInterrupt.h>
	
	void (*irq_handler_function)(void);
	
	void timed_irq_handler() {
		irq_handler_function();
	}
	
	void set_timed_irq(void (*function), int millis) {
		ITimer2.init();
		irq_handler_function = function;
		ITimer2.attachInterruptInterval(millis, timed_irq_handler);
	}
	
	#include <SoftwareSerial.h>
	SoftwareSerial serial_interface(2, 3);
	
	void debug_init(int baudrate) {
		Serial.begin(baudrate);
	}
	
	void debug_print(const char* string) {
		Serial.print(string);
	}
	
	const char* debug_read_line() {
		String string = Serial.readStringUntil("\n");
		return string.c_str();
	}
	
	void serial_init(int baudrate) {
		serial_interface.begin(baudrate);
	}
	
	void serial_print(const char* command) {
		kernel_preemption_enabled = 0;
		serial_interface.print(command);
		kernel_preemption_enabled = 1;
	}
	
	void serial_send_byte(int b) {
		kernel_preemption_enabled = 0;
		serial_interface.write(b);
		kernel_preemption_enabled = 1;
	}
	
	const char* serial_read_line() {
		kernel_preemption_enabled = 0;
		String string = serial_interface.readStringUntil("\n");
		kernel_preemption_enabled = 1;
		return string.c_str();
	}
	
	char button_matrix[5][3] = {
		{'<', 'E', '>'},
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'}
	};
	
	int port_matrix_out[3];
	int port_matrix_in[5];
	
	int buttons_pressed[3][5];
	
	void matrix_init(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7) {
		port_matrix_out[0] = p0;
		port_matrix_out[1] = p1;
		port_matrix_out[2] = p2;
		port_matrix_in[0] = p3;
		port_matrix_in[1] = p4;
		port_matrix_in[2] = p5;
		port_matrix_in[3] = p6;
		port_matrix_in[4] = p7;
		
		for(int i = 0; i < 3; i++) {
			gpio_init(port_matrix_out, OUTPUT);
		}
		
		for(int i = 0; i < 5; i++) {
			gpio_init(port_matrix_in, INPUT);
		}
	}
	
	void matrix_refresh_pressed_buttons() {
		for(int x = 0; x < 3; x++) {
			gpio_out(port_matrix_out[x], 1);
			
			for(int y = 0; y < 5; y++) {
				if(gpio_in(port_matrix_in[y]) == HIGH) {
					buttons_pressed[x][y] = 1;
				} else buttons_pressed[x][y] = 0;
			}
			
			gpio_out(port_matrix_out[x], 0);
		}
	}
#endif
