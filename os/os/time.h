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
 * Time functions for arcticOS
 * 
 * This handles a few things:
 * - Stores the time since the Epoch
 * 		- Worth noting that this doesn't use the Unix epoch, but rather it uses Jan 1, 2020 (2020 is when the project started)
 * - Returns the second, minute, hour, day, month, year
 * 
 * Note that it DOESN't handle stupid human things like timezones and DST.
 * 
 * TODO:
 * - Add support for the stupid NA time thing (mm/dd/yyyy)
 */

#ifndef TIME_H
	#define TIME_H
	
	int time_since_epoch = 0; // Default to the epoch
	int time_since_second = 0; // Time since this second started
	
	void time_tick() {
		time_since_second += 1;
		
		if(time_since_second == 100) {
			time_since_epoch ++;
			time_since_second = 0;
		}
		
		return;
	}
	
	int time_get_year() {
		int year = 0;
		year += time_since_epoch / 31556926;
		return year;
	}
	
	int time_get_month() {
		int month = 0;
		
		int time_without_years = time_since_epoch - (time_get_year() * 31556926);
		month += time_without_years / 2629743;
		
		return month;
	}
	
	int time_get_day() {
		int day = 0;
		
		int time_without_months = time_since_epoch - (time_get_year() * 31556926) - (time_get_month() * 2629743);
		day += time_without_months / 86400;
		
		return day;
	}
	
	int time_get_hour24() {
		int hour = 0;
		
		int time_without_days = time_since_epoch - (time_get_year() * 31556926) - (time_get_month() * 2629743) - (time_get_day() * 86400);
		hour += time_without_days / 3600;
	
		return hour;
	}
	
	int time_get_hour12() {
		int hour = 0;
		
		int time_without_days = time_since_epoch - (time_get_year() * 31556926) - (time_get_month() * 2629743) - (time_get_day() * 86400);
		hour += time_without_days / 3600;
		
		if(hour > 12) hour -= 12;
	
		return hour;
	}
	
	const char* time_get_ampm() {
		if(time_get_hour24() > 12) return "PM";
		else return "AM";
	}
	
	int time_get_minute() {
		int minute = 0;
		
		int time_without_hours = time_since_epoch - (time_get_year() * 31556926) - (time_get_month() * 2629743) - (time_get_day() * 86400) - (time_get_hour24() * 3600);
		minute += time_without_hours / 60;
	
		return minute;
	}
	
	int time_get_second() {
		int second = 0;
		
		int time_without_minutes = time_since_epoch - (time_get_year() * 31556926) - (time_get_month() * 2629743) - (time_get_day() * 86400) - (time_get_hour24() * 3600) - (time_get_minute() * 60);
		second += time_without_minutes;
	
		return second;
	}
	
	int time_get_human_year() { return time_get_year() + 2020; }
	int time_get_human_month() { return time_get_month() + 1; }
	int time_get_human_day() { return time_get_day() + 1; }
	
	// This is way too fucking complicated, why can't people just count hours at 0
	int time_get_human_hour12() { 
		int hour = time_get_hour12();
		if(hour == 0) hour = 12;
		return hour;
	}
	
	String time_get_date_string() {
		String date_string = "";
		date_string.concat(time_get_human_day());
		date_string.concat("/");
		date_string.concat(time_get_human_month());
		date_string.concat("/");
		date_string.concat(time_get_human_year());
		
		return date_string;
	}
	
	String time_get_time24_string() {
		String time_string = "";
		if(time_get_hour24() < 10) time_string.concat(0);
		time_string.concat(time_get_hour24());
		time_string.concat(":");
		if(time_get_minute() < 10) time_string.concat(0);
		time_string.concat(time_get_minute());
		
		return time_string;
	}
	
	String time_get_time12_string() {
		String time_string = "";
		if(time_get_human_hour12() < 10) time_string.concat(0);
		time_string.concat(time_get_human_hour12());
		time_string.concat(":");
		if(time_get_minute() < 10) time_string.concat(0);
		time_string.concat(time_get_minute());
		time_string.concat(" ");
		time_string.concat(time_get_ampm());
		
		return time_string;
	}
#endif
