# arcticOS
# Copyright (c) 2021 Johnny Stene <jhonnystene@protonmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

import os, requests, base64

class KeyInput():
    def __init__(self):
        #super(KeyDriver, self).__init__()
        self.inputBuffer = []
    
    def make_request(self, path):
        r = requests.get("http://localhost:3503/input/" + base64.b64encode(bytes(path, "ascii")).decode("ascii"))
        return r.text

    def getKey(self):
        text = ""
        while(text == ""):
            text = self.make_request("S")
        return text

    # This returns true if a character is "printable"
    def isPrintable(self, char):
        if(char in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$%^&*()_+[]\\{}|;\':\",./<>? "):
            return True
        return False

    # This gets a "printable" character only.
    def getPrintableCharacter(self):
        while True:
            char = self.getKey()

            if(self.isPrintable(char)):
                return char

# This extends the DisplayDriver so it can provide shared functionality across hardware.
class Display():
    def __init__(self):
        #super(Display, self).__init__()
        self.width = 264
        self.height = 176

    def make_request(self, path):
        r = requests.get("http://localhost:3502/display/" + base64.b64encode(bytes(path, "ascii")).decode("ascii"))
        return r.text

    def clear(self):
        self.make_request("C")

    def refresh(self):
        self.make_request("R")
        
    def getTextBounds(self, text, size):
        if(text == ""):
            return [0, 0]
        bounds = self.make_request("B\r" + text + "\r" + str(size))
        bounds = bounds.split(",")
        for i in range(0, len(bounds)):
            bounds[i] = int(bounds[i].strip().strip("\n"))
        return bounds

    def drawText(self, text, size, x, y):
        self.make_request("T\r" + text + "\r" + str(size) + "\r" + str(x) + "\r" + str(y))

    def drawTextCentered(self, text, size, x, y):
        self.make_request("TC\r" + text + "\r" + str(size) + "\r" + str(x) + "\r" + str(y))
        
    def drawRect(self, x, y, width, height):
        self.make_request("R\r" + str(x) + "\r" + str(y) + "\r" + str(width) + "\r" + str(height))

    def fillRect(self, x, y, width, height):
        self.make_request("RF\r" + str(x) + "\r" + str(y) + "\r" + str(width) + "\r" + str(height))

    def drawLine(self, x1, y1, x2, y2):
        self.make_request("L\r" + str(x1) + "\r" + str(y1) + "\r" + str(x2) + "\r" + str(y2))

    # Draws app name and a fancy line at the top of the screen.
    def drawAppHeader(self, appName):
        self.drawText(appName, 20, 10, 3)
        self.drawLine(0, 30, self.width, 30)

    # Draws navigation information and a fancy line at the bottom of the screen.
    def drawNavbar(self, leftText="Back", centerText="OK", rightText="Menu"):
        self.drawLine(0, self.height - 20, self.width, self.height - 20)

        centerBounds = self.getTextBounds(centerText, 18)
        rightBounds = self.getTextBounds(rightText, 18)
        
        self.drawText(leftText, 15, 10, self.height - 18)
        self.drawText(centerText, 15, (self.width / 2) - (centerBounds[0] / 2), self.height - 18)
        self.drawText(rightText, 15, self.width - 10 - rightBounds[0], self.height - 18)
    
    # Draws a basic button.
    def drawButton(self, text, x, y, width, height, selected=False):
        self.drawRect(x, y, width, height)
        self.drawRect(x + 1, y + 1, width - 2, height - 2)
        
        if(selected):
            self.drawRect(x + 2, y + 2, width - 4, height - 4)

        self.drawTextCentered(text, 20, x + (width / 2), y + (height / 2))

    # Draws a basic text box.
    def drawTextBox(self, text, x, y, width, height=35, selected=False):
        self.drawRect(x, y, width, height)
        self.drawRect(x + 1, y + 1, width - 2, height - 2)
        
        if(selected):
            self.drawRect(x + 2, y + 2, width - 4, height - 4)

        self.drawText(text, 20, x + 5, y + 5)

    # Draws a basic toggle switch.
    def drawToggleSwitch(self, x, y, checked=False):
        self.drawRect(x, y, 20, 20)
        self.drawRect(x + 1, y + 1, 18, 18)
        
        if(checked):
            self.fillRect(x + 5, y + 5, 10, 10)
    
    # Draws a list of items.
    def drawList(self, list, selected=0):
        startIndex = selected - 3
        if(startIndex < 0):
            startIndex = 0
        list = list[startIndex:startIndex+3]
        for item in range(0, len(list)):
            self.drawText(list[item], 18, 10, 38 + (30 * item))
            self.drawLine(10, 30 + (30 * (item + 1)), self.width - 10, 30 + (30 * (item + 1)))

        startIndex = selected - startIndex
        self.drawLine(10, 59 + (30 * startIndex), self.width - 10, 59 + (30 * startIndex))
    
    # Draws a large block of text.
    def drawTextBlock(self, block, cursorX=0, cursorY=0, drawCursor=False):
        list = block.split("\n")
        startIndex = cursorY - 4
        if(startIndex < 0):
            startIndex = 0
        list = list[startIndex:startIndex+5]
        for item in range(0, len(list)):
            self.drawText(list[item], 18, 10, 38 + (20 * item))
        
        startIndex = cursorY - startIndex
        self.drawLine(10 + self.getTextBounds(list[startIndex][:cursorX], 18)[0], 38 + (20 * startIndex), 10 + self.getTextBounds(list[startIndex][:cursorX], 18)[0], 58 + (20 * startIndex))

    # Returns a single line of text.
    def getString(self, KeyInput, info, string=""):
        editing = True
        while editing:
            self.drawAppHeader(info)
            self.drawNavbar("Cancel", "", "Finish")
            self.drawTextBox(string, 10, 40, self.width-20, selected=True)
            self.refresh()

            while True:
                key = KeyInput.getKey()
                if(key == "end"):
                    editing = False
                    string = ""
                    break
                elif(KeyInput.isPrintable(key)): # TODO: add all characters
                    string = string + key
                    break
                elif(key == "back"):
                    string = string[:len(string) - 2]
                    break
                elif(key == "answer"):
                    editing = False
                    break
        return string

    # Returns a block of text.
    def getMultilineString(self, KeyInput, info, string=""):
        editing = True
        cursorX = 0
        cursorY = 0
        while editing:
            self.drawAppHeader(info)
            self.drawNavbar()
            self.drawTextBlock(string, cursorX, cursorY, True)
            self.refresh()

            while True:
                key = KeyInput.getKey()
                if(key == "end"):
                    editing = False
                    break
                elif(key == "down"):
                    cursorY += 1
                    if(cursorY == len(string.split("\n"))):
                        cursorY = len(string.split("\n")) - 1
                    break
                elif(key == "up"):
                    cursorY -= 1
                    if(cursorY < 0):
                        cursorY = 0
                    break
                elif(key == "right"):
                    cursorX += 1
                    if(cursorX > len(string.split("\n")[cursorY])):
                        cursorX = len(string.split("\n")[cursorY]) - 1
                    break
                elif(key == "left"):
                    cursorX -= 1
                    if(cursorX < 0):
                        cursorX = 0
                    break
                elif(KeyInput.isPrintable(key)): # TODO: add all characters
                    lines = string.split("\n")
                    lines[cursorY] = lines[cursorY][:cursorX] + key + lines[cursorY][cursorX:]
                    cursorX += 1
                    string = ""
                    for line in lines:
                        string = string + line + "\n"
                    break
                elif(key == "enter"):
                    lines = string.split("\n")
                    lines[cursorY] = lines[cursorY][:cursorX] + "\n" + lines[cursorY][cursorX:]
                    cursorX = 0
                    cursorY += 1
                    string = ""
                    for line in lines:
                        string = string + line + "\n"
                    break
                elif(key == "back"):
                    lines = string.split("\n")
                    lines[cursorY] = lines[cursorY][:cursorX - 1] + lines[cursorY][cursorX:]
                    cursorX -= 1
                    string = ""
                    for line in lines:
                        string = string + line + "\n"
                    break
        return string
    
    # Returns the user's selection from a list.
    def getMenuSelection(self, KeyInput, info, menuItems, specialButton=""):
        inMenu = True
        itemSelected = 0
        while inMenu:
            self.drawAppHeader(info)
            self.drawNavbar("Back", "Select", specialButton)
            self.drawList(menuItems, itemSelected)
            self.refresh()

            # Input handling goes here
            while True:
                key = KeyInput.getKey()

                if(key == "down"):
                    itemSelected += 1
                    if(itemSelected == len(menuItems)):
                        itemSelected = 0
                    break
                elif(key == "up"):
                    itemSelected -= 1
                    if(itemSelected < 0):
                        itemSelected = len(menuItems) - 1
                    break
                elif(key == "ok"):
                    inMenu = False
                    break
                elif(key == "end"):
                    inMenu = False
                    itemSelected = -1
                    break
                elif(key == "answer" and specialButton != ""):
                    inMenu = False
                    itemSelected = -2
                    break
        return itemSelected