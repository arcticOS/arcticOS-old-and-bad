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
    inApp = True
    while inApp:
        notes = UserSettings.getKey("Notes")
        noteNames = []
        for note in notes:
            noteNames.append(note["title"])
        selection = PhoneDisplay.getMenuSelection(KeyInput, "Select Note", noteNames, specialButton="New")
        if(selection == -1):
            inApp = False
        elif(selection == -2):
            createNote(notes, PhoneDisplay, KeyInput, BuildInfo, UserSettings)
        elif(selection >= 0):
            editNote(notes, selection, PhoneDisplay, KeyInput, BuildInfo, UserSettings)

def createNote(noteList, PhoneDisplay, KeyInput, BuildInfo, UserSettings):
    editing = True
    noteData = {"title": "", "content": ""}
    noteData["title"] = PhoneDisplay.getString(KeyInput, "Note Name")
    if(noteData["title"] == ""):
        noteData["title"] = "Untitled Note"
    noteList.append(noteData)
    UserSettings.setKey("Notes", noteList)
    editNote(noteList, len(noteList) - 1, PhoneDisplay, KeyInput, BuildInfo, UserSettings)

def editNote(noteList, noteSelected, PhoneDisplay, KeyInput, BuildInfo, UserSettings):
    noteList[noteSelected]["content"] = PhoneDisplay.getMultilineString(KeyInput, noteList[noteSelected]["title"], noteList[noteSelected]["content"])
    UserSettings.setKey("Notes", noteList)