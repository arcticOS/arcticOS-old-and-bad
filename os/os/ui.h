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
 * UI Stuff for arcticOS
 */

#ifndef UI_H
	#define UI_H
	
	const char* ui_seven_segment_font[10] = {"1110111","0010010","1011101","1011011","0111010","1101011","1101111","1010010","1111111","1111011"};

	void ui_seven_segment_horizontal(int x, int y) {
		board_screen_rect(x, y, 41, 3);
		/*board_screen_rect(x + 2, y + 4, 35, 1);

		board_screen_rect(x + 1, y + 1, 36, 1);
		board_screen_rect(x + 1, y + 3, 36, 1);

		board_screen_rect(x, y + 2, 37, 1);*/
	}

	void ui_seven_segment_vertical(int x, int y) {
		board_screen_rect(x, y, 3, 41);
		/*board_screen_rect(x + 4, y + 2, 1, 35);

		board_screen_rect(x + 1, y + 1, 1, 36);
		board_screen_rect(x + 3, y + 1, 1, 36);

		board_screen_rect(x + 2, y + 2, 1, 37);*/
	}

	void ui_seven_segment(int x, int y, char c) {
		if(c < '0' || c > '9') return; // We can only 7-segment numbers
		const char* character = ui_seven_segment_font[c - '0'];

		// Do horizontal ones first
		if(character[0] == '1') ui_seven_segment_horizontal(x, y);
		if(character[3] == '1') ui_seven_segment_horizontal(x, y + 38);
		if(character[6] == '1') ui_seven_segment_horizontal(x, y + 76);

		// Now vertical (left)
		if(character[1] == '1') ui_seven_segment_vertical(x, y);
		if(character[4] == '1') ui_seven_segment_vertical(x, y + 38);

		// Vertical right
		if(character[2] == '1') ui_seven_segment_vertical(x + 38, y);
		if(character[5] == '1') ui_seven_segment_vertical(x + 38, y + 38);
	}
#endif
