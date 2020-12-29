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
 * Fact: The default C string functions can eat my whole ass.
 */

#ifndef BETTERSTR
	#define BETTERSTR
	
	const char* str_substr(const char* str, int index) {
		return &str[index];
	}
	
	int str_startswith(const char* str1, const char* str2) {
		if(strlen(str2) > strlen(str1)) return 0;// str1 can't possibly start with str2 if str2 is longer
		
		for(int i = 0; i < strlen(str2); i++) {
			if(str1[i] != str2[i]) return 0;
		}
		
		return 1;
	}
	
	int str_contains(const char* str1, const char* str2) {
		if(strlen(str2) > strlen(str1)) return 0; // str1 can't possibly contain str2 if str2 is longer
		
		for(int i = 0; i < strlen(str1) - strlen(str2); i++) { // len(1) - len(2) because there's no point checking anywhere where the string can't fit
			if(str_startswith(str_substr(str1, i), str2)) return 1;
		}
		
		return 0;
	}
#endif
