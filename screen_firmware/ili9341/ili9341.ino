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

int screenChunks[8] = {255, 255, 255, 255, 255, 255, 255, 255}; // Set these all to 255 on boot so we clear everything
int chunkWidth = 30;
int chunkHeight = 40;

int textLine = 0;

// Adafruit's shitty library can't handle hardware SPI for some fucking reason so we have to stick w/ software
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

int textColor = 0;

void setRectUsed(int x, int y, int w, int h) {
	int startBoxX = x / 30;
	int endBoxX = ((x+w)/30)+1;
	int startBoxY = y / 40;
	int endBoxY = ((y+h)/30)+1;

	for(int x = startBoxX; x < endBoxX; x++) {
		for(int y = startBoxY; y < endBoxY; y++) {
			screenChunks[y] = screenChunks[y] | (1 << x);
		}
	}
}

void setRectUnused(int x, int y, int w, int h) {
	int startBoxX = x / 30;
	int endBoxX = ((w - x) / 30) + 1;
	int startBoxY = y / 40;
	int endBoxY = ((h - y) / 40) + 1;
	for(int x = startBoxX; x < endBoxX; x++) {
		for(int y = startBoxY; y < endBoxY; y++) {
			screenChunks[y] = screenChunks[y] & ~(1 << x);
		}
	}
}

void clearScreen() {
	//tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(0, 0);
	textLine = 0;

	// Clear only the used areas to save time
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x++) {
			int clear = (screenChunks[y] & (1 << x)) >> x; // Get the Xth bit of our current row
			if(clear) {
				tft.fillRect(x * chunkWidth, y * chunkHeight, chunkWidth, chunkHeight, ILI9341_WHITE);
			}
		}

		screenChunks[y] = 0;
	}
}

void drawText(int textSize, String text) {
	int startX = 0;
	int startY = textLine;
	int endX = 240;
	int endY = (((8 * textSize) * text.length()) / 240) * (8 * textSize);
	textLine = endY;

	setRectUsed(startX, startY, endX, endY);

	tft.setTextSize(textSize);
	if(textColor == 0) tft.setTextColor(ILI9341_BLACK);
	else tft.setTextColor(ILI9341_WHITE);
	tft.println(text);
}

void drawRectangle(int x, int y, int w, int h) {
	tft.fillRect(x, y, w, h, ILI9341_BLACK);
	setRectUsed(x, y, w, h);
}

void clearRectangle(int x, int y, int w, int h) {
	tft.fillRect(x, y, w, h, ILI9341_WHITE);
	//setRectUnused(x, y, w, h);
}

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(300000);
	tft.begin();
	//clearScreen();

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
	} else if(inString.charAt(0) == 'P') { // Position
		// Structure: P(3 characters representing an integer)(3 characters representing an integer)
		int x = inString.substring(1, 4).toInt();
		int y = inString.substring(4, 7).toInt();
		tft.setCursor(x, y);
		textLine = y;
		Serial.println("R");
	} else if(inString.charAt(0) == 'A') { // Clear area
		// Structure: A(3 characters representing an integer)(3 characters representing an integer)(3 characters representing an integer)(3 characters representing an integer)
		int x = inString.substring(1, 4).toInt();
		int y = inString.substring(4, 7).toInt();
		int w = inString.substring(7, 10).toInt();
		int h = inString.substring(10, 13).toInt();
		clearRectangle(x, y, w, h);
		Serial.println("R");
	} else if(inString.charAt(0) == 'V') { // Invert text color
		if(textColor) textColor = 0;
		else textColor = 1;
		Serial.println("R");
	} else if(inString.charAt(0) == 'Q') { // Set text color
		if(inString.charAt(0) == 'W') textColor = 0;
		else textColor = 1;
		Serial.println("R");
	}
}
