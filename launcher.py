#!/usr/bin/env python3
#
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

import json

from arcticOS import HID, App, PreferenceStorage

# TODO: Remove this to prevent settings reset
PreferenceStorage.factoryReset()

global BuildInfo, UserSettings
BuildInfo = PreferenceStorage.PreferenceStorage("settings/build.json")
UserSettings = PreferenceStorage.PreferenceStorage("settings/user.json")

global PhoneDisplay, KeyInput
PhoneDisplay = HID.Display()
KeyInput = HID.KeyInput()

if(__name__ == "__main__"):
    # Launch OOBE if needed
    if(UserSettings.getKey("OOBEComplete") == 0):
        print("Launching OOBE...")
        App.launchPath("apps/system/oobe.py", PhoneDisplay, KeyInput, BuildInfo, UserSettings)

        UserSettings.setKey("OOBEComplete", 1)

    for app in App.getAllApps("apps"):
        print(app.name + " + " + app.fileName)

    PhoneDisplay.refresh()

    while True:
        PhoneDisplay.drawAppHeader("Launcher")
        PhoneDisplay.drawRect(40, 40, 40, 40)
        PhoneDisplay.fillRect(100, 40, 40, 40)
        PhoneDisplay.drawButton("Button!", 40, 85, 60, 30)
        PhoneDisplay.refresh()

        # Input handling goes here
        while True:
            key = KeyInput.getKey()

            if(key == "ok"):
                break

