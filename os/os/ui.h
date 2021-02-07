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
	
	// This is fucking retarded. The screen overscans so bad that pixel 0 is 60 pixels above the top of the screen.
	int ui_overscan_y = 15; 
	
	/*
	 * 0 - No screen
	 * 1 - Home screen
	 * 2 - Dialer
	 */
	int ui_current_screen = 0;
	
	void ui_draw_navbar(const char* left, const char* middle, const char* right) {
		board_screen_text(10, board_screen_height, 0, 6, left);
		board_screen_text(120, board_screen_height, 0, 5, middle);
		board_screen_text(230, board_screen_height, 0, 4, right);
	}
	
	// Variables for calling screen (ID 2)
	String ui_call_number = "";
	
	void ui_do_redraw() {
		// UI code for all the screens go here
		
		board_screen_clear();
		
		switch(ui_current_screen) {
			case(1):
				// Draw clock
				board_screen_text(board_screen_width / 2, 50 + ui_overscan_y, 1, 1, time_get_time12_string().c_str());
				board_screen_text(board_screen_width / 2, 75 + ui_overscan_y, 0, 1, time_get_date_string().c_str());
				
				ui_draw_navbar("Dial", "Menu", "");
				
				break;
			case(2):
				ui_draw_navbar("Call", "Back", "Delete");
				break;
		}
	}
	
	void ui_switch_screen(int new_screen) {
		debug_printf("Switching to screen %d.\n", new_screen);
		ui_current_screen = new_screen;
		ui_do_redraw();
	}
	
	void ui_do_loop() {
		// Input handling code and redraw calls for all the screens go here
		
		if(ui_current_screen == 0) {
			ui_switch_screen(1);
		} else if(ui_current_screen == 1) {
			char c = os_wait_for_key();
			if(c == 'A')
				ui_switch_screen(2);
		} else if(ui_current_screen == 2) {
			board_debug_print(ui_call_number.c_str());
			board_debug_print("\n");
			
			char c = os_wait_for_key();
			
			if(c == 'A') return; // Calling isn't implemented yet
			if(c == 'O') {
				ui_call_number = "";
				ui_switch_screen(1);
			}
			if(c == 'E') {
				ui_call_number.remove(ui_call_number.length() - 1);
				board_screen_rect(0, 50, board_screen_width, 50, 0);
				board_screen_text(board_screen_width / 2, 50 + ui_overscan_y, 0, 1, ui_call_number.c_str());
			}
			
			if(c >= '0' && c <= '9' && ui_call_number.length() < 15) { // Phone numbers usually shouldn't be more than 15 characters.
				ui_call_number.concat(c);
				board_screen_rect(0, 50, board_screen_width, 50, 0);
				board_screen_text(board_screen_width / 2, 50 + ui_overscan_y, 0, 1, ui_call_number.c_str());
			}
		}
	}

	void ui_do_timed_refresh() {
			if(ui_current_screen == 1) {
				if(time_get_second() == 0) {
					// Clear clock area
					board_screen_rect(0, 45 + ui_overscan_y, board_screen_width, 100, 0);
					
					// Draw clock
					board_screen_text(board_screen_width / 2, 50 + ui_overscan_y, 1, 1, time_get_time12_string().c_str());
					board_screen_text(board_screen_width / 2, 75 + ui_overscan_y, 0, 1, time_get_date_string().c_str());
				}
			}
	}
#endif
