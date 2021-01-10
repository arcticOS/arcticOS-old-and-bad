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
 * Screen controller firmware for ILI9341-based screens
 * This should be an Arduino Nano connected via SPI to the ILI9341.
 */

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9							
#define TFT_CS 10						
#define TFT_RST 8
#define TFT_MISO 11				 
#define TFT_MOSI 12
#define TFT_CLK 13	 

// Adafruit's shitty library can't handle hardware SPI for some fucking reason so we have to stick w/ software
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void clearScreen() {
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(0, 0);
}

void drawText(int textSize, String text) {
	tft.setTextSize(textSize);
	tft.setTextColor(ILI9341_BLACK);
	tft.println(text);
}

void drawRectangle(int x, int y, int w, int h) {
	tft.fillRect(x, y, w, h, ILI9341_BLACK);
}

void setup() {
	Serial.begin(9600);
	tft.begin();
	clearScreen();

	Serial.println("R");
}


void loop(void) {
	String inString = Serial.readStringUntil('\n');
	if(inString.charAt(0) == 'I') { // Info request
		// Structure: I
		Serial.println("W240H320V001");
		Serial.println("R");
	} else if(inString.charAt(0) == 'C') { // Clear Screen
		// Structure: C
		clearScreen();
		Serial.println("R");
	} else if(inString.charAt(0) == 'R') { // Rectangle
		// Structure: R(3 characters representing an integer)(3 characters representing an integer)(3 characters representing an integer)(3 characters representing an integer)
		int x = inString.substring(1, 4).toInt();
		int y = inString.substring(4, 7).toInt();
		int w = inString.substring(7, 10).toInt();
		int h = inString.substring(10, 13).toInt();
		drawRectangle(x, y, w, h);
		Serial.println("R");
	} else if(inString.charAt(0) == 'T') { // Text
		// Structure: T(1 character representing text size from 1-4)(String)
		int size = (int)inString.charAt(1) - 48;
		String str = inString.substring(2);
		drawText(size, str);
		Serial.println("R");
	}
}
