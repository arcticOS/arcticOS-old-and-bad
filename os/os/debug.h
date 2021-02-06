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
 * "High-level" functions for the default debug interface.
 */
 
#include <stdarg.h>
 
#ifndef DEBUG_H
	#define DEBUG_H
	
	int debug_printf(char* string, ...) {
		va_list args;
		
		int i = 0;
		va_start(args, string);
		
		while(string[i]) {
			if(string[i] == '%') {
				i ++;
				
				if(string[i] == '%') {
					board_debug_print("%");
				} else if(string[i] == 'c') {
					board_debug_print((char) va_arg(args, int));
				} else if(string[i] == 'd') {
					board_debug_print(va_arg(args, int));
				} else if(string[i] == 'x') {
					board_debug_print((const char*) va_arg(args, int));
				}
			} else {
				board_debug_print(string[i]);
			}
			
			i ++;
		}
	}
#endif
