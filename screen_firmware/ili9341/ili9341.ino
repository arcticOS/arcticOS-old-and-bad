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
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#define DISPLAYWIDTH 240
#define DISPLAYHEIGHT 320

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TFT_MISO 11
#define TFT_MOSI 12
#define TFT_CLK 13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define CHUNK_WIDTH 30
#define CHUNK_HEIGHT 40
int usedChunks[8] = {255, 255, 255, 255, 255, 255, 255, 255};

int isChunkUsed(int x, int y) {
	int mask = (1 << x);
	int used = usedChunks[y] & mask;
	return used;
}

void setChunkUsed(int x, int y, int used) {
	if(used) usedChunks[y] = usedChunks[y] | (1 << x);
	else usedChunks[y] = usedChunks[y] & ~(1 << x);
}

void setAreaUsed(int x, int y, int width, int height, int used) {
	x = (x / CHUNK_WIDTH) - 1;
	y = (y / CHUNK_HEIGHT) - 1;
	
	width = (width / CHUNK_WIDTH) + 3;
	height = (height / CHUNK_HEIGHT) + 4;
	
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	if(x + width > 8) width = 8 - x;
	if(y + height > 8) height = 8 - y;
	
	for(int i = y; i < y + height; i++) {
		if(i > DISPLAYHEIGHT / CHUNK_HEIGHT) break;
		
		for(int i2 = x; i2 < x + width; i2++) {
			if(i2 > DISPLAYWIDTH / CHUNK_WIDTH) break;
			
			setChunkUsed(i2, i, used);
		}
	}
}

void clearScreen() {
	tft.setCursor(0, 0);
	
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x++) {
			if(isChunkUsed(x, y)) {
				tft.fillRect(x * CHUNK_WIDTH, y * CHUNK_HEIGHT, CHUNK_WIDTH, CHUNK_HEIGHT, ILI9341_WHITE);
				setChunkUsed(x, y, 0);
			}
		}
	} 
}

#define ALIGN_NW 0
#define ALIGN_N 1
#define ALIGN_NE 2
#define ALIGN_E 3
#define ALIGN_SE 4
#define ALIGN_S 5
#define ALIGN_SW 6
#define ALIGN_W 7
#define ALIGN_C 8

void drawText(int x, int y, int size, int align, String text) {
	if(size == 0) tft.setFont(&FreeSans12pt7b);
	else if(size == 1) tft.setFont(&FreeSans18pt7b);
	
	int16_t dx, dy;
	uint16_t width, height;
	
	tft.getTextBounds(text, 0, 0, &dx, &dy, &width, &height);
	
	dx = x;
	dy = y;
	
	switch(align) {
		case(ALIGN_NW):
			dx = x;
			dy = y;
			break;
		case(ALIGN_N):
			dx = x - (width / 2);
			break;
		case(ALIGN_NE):
			dx = x - width;
			break;
		case(ALIGN_E):
			dx = x - width;
			dy = y - (height / 2);
			break;
		case(ALIGN_SE):
			dx = x - width;
			dy = y - height;
			break;
		case(ALIGN_S):
			dx = x - (width / 2);
			dy = y - height;
			break;
		case(ALIGN_SW):
			dy = y - height;
			break;
		case(ALIGN_W):
			dy = y - (height / 2);
			break;
		case(ALIGN_C):
			dx = x - (width / 2);
			dy = y - (height / 2);
			break;
	}
	
	setAreaUsed(dx, dy, width, height, 1);
	
	tft.setCursor(dx, dy);
	tft.setTextColor(ILI9341_BLACK);
	tft.print(text);
}

void setup() {
	Serial.begin(9600);
	Serial.setTimeout(300000);
	tft.begin();
	tft.setTextColor(ILI9341_BLACK);
	Serial.println("R");
}

void loop() {
	String command = Serial.readStringUntil('\n');
	
	if(command.charAt(0) == 'W') {
		Serial.println(DISPLAYWIDTH);
	} else if(command.charAt(0) == 'H') {
		Serial.println(DISPLAYHEIGHT);
	} else if(command.charAt(0) == 'C') {
		clearScreen();
	} else if(command.charAt(0) == 'T') {
		int x = command.substring(1, 4).toInt();
		int y = command.substring(4, 7).toInt() + 10;
		int size = (int) command.charAt(7) - 48;
		int align = (int) command.charAt(8) - 48;
		String string = command.substring(9);
		drawText(x, y, size, align, string);
	} else if(command.charAt(0) == 'R') {
		int x = command.substring(1, 4).toInt();
		int y = command.substring(4, 7).toInt();
		int w = command.substring(7, 10).toInt();
		int h = command.substring(10, 13).toInt();
		int color = (int) command.charAt(13) - 48;
		setAreaUsed(x, y, w, h, 1);
		
		if(color == 1) tft.fillRect(x, y, w, h, ILI9341_BLACK);
		else tft.fillRect(x, y, w, h, ILI9341_WHITE);
	}
	Serial.println(command);
	//Serial.println("R");
}
