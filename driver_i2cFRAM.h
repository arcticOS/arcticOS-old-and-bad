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
 * I2C FRAM driver
 */

#ifndef DRIVER_FLASH
	#define DRIVER_FLASH
	#define DRIVER_FLASH_NAME "I2C FRAM\n"
	
	#ifndef BOARD_HAS_I2C
		#error "The I2C FRAM driver requires the board to have I2C functionality!"
	#endif
	
	#define FLASH_ADDRESS 0x50
	
	uint8_t flash_get_byte(uint16_t addr) {
		// Tell the chip where we want data from
		i2c_start(FLASH_ADDRESS);
		i2c_send_byte(addr >> 8);
		i2c_send_byte(addr & 0xFF);
		i2c_end();
		
		// Get the byte
		i2c_request(FLASH_ADDRESS, (uint8_t) 1);
		return (uint8_t) i2c_get();
	}
	
	void flash_set_byte(uint16_t addr, uint8_t value) {
		i2c_start(FLASH_ADDRESS);
		i2c_send_byte(addr >> 8);
		i2c_send_byte(addr & 0xFF);
		i2c_send_byte(value);
		i2c_end();
	}
#endif
