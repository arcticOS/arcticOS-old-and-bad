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

def runApp(PhoneDisplay, KeyInput, BuildInfo, UserSettings):
    PhoneDisplay.clear()

    notes = UserSettings.getKey("Notes")
    noteNames = []
    for note in notes:
        noteNames.append(note["title"])
    noteSelected = 0

    inApp = True
    while inApp:
        PhoneDisplay.drawAppHeader("Notes")
        PhoneDisplay.drawNavbar("Exit", "Edit", "")
        PhoneDisplay.drawList(noteNames, noteSelected)
        PhoneDisplay.refresh()
        
        while True:
            key = KeyInput.getKey()

            if(key == "down"):
                noteSelected += 1
                if(noteSelected == len(noteNames)):
                    noteSelected = 0
                break
            elif(key == "up"):
                noteSelected -= 1
                if(noteSelected < 0):
                    noteSelected = len(noteNames) - 1
                break
            elif(key == "ok"):
                editNote(notes, noteSelected, PhoneDisplay, KeyInput, BuildInfo, UserSettings)
                noteSelected = 0
                break
            elif(key == "end"):
                inApp = False
                break

def editNote(noteList, noteSelected, PhoneDisplay, KeyInput, BuildInfo, UserSettings):
    editing = True
    cursorX = 0
    cursorY = 0
    while editing:
        PhoneDisplay.drawAppHeader("Notes")
        PhoneDisplay.drawNavbar()
        PhoneDisplay.drawTextBlock(noteList[noteSelected]["content"], cursorX, cursorY, True)
        PhoneDisplay.refresh()

        while True:
            key = KeyInput.getKey()
            if(key == "end"):
                editing = False
                break
            elif(key == "down"):
                cursorY += 1
                if(cursorY == len(noteList[noteSelected]["content"].split("\n"))):
                    cursorY = len(noteList[noteSelected]["content"].split("\n")) - 1
                break
            elif(key == "up"):
                cursorY -= 1
                if(cursorY < 0):
                    cursorY = 0
                break
            elif(key == "right"):
                cursorX += 1
                if(cursorX == len(noteList[noteSelected]["content"].split("\n")[cursorY])):
                    cursorX = len(noteList[noteSelected]["content"].split("\n")[cursorY]) - 1
                break
            elif(key == "left"):
                cursorX -= 1
                if(cursorX < 0):
                    cursorX = 0
                break
            elif(key in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456790 "): # TODO: add all characters
                lines = noteList[noteSelected]["content"].split("\n")
                lines[cursorY] = lines[cursorY][:cursorX] + key + lines[cursorY][cursorX:]
                cursorX += 1
                noteList[noteSelected]["content"] = ""
                for line in lines:
                    noteList[noteSelected]["content"] = noteList[noteSelected]["content"] + line + "\n"
                break
            elif(key == "back"):
                lines = noteList[noteSelected]["content"].split("\n")
                lines[cursorY] = lines[cursorY][:cursorX - 1] + lines[cursorY][cursorX:]
                cursorX -= 1
                noteList[noteSelected]["content"] = ""
                for line in lines:
                    noteList[noteSelected]["content"] = noteList[noteSelected]["content"] + line + "\n"
                break
    UserSettings.setKey("Notes", noteList)