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

global BuildInfo, UserSettings
BuildInfo = PreferenceStorage.PreferenceStorage("settings/build.json")
if(BuildInfo.getKey("always_factory_reset")):
    PreferenceStorage.factoryReset()

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

    apps = App.getAllApps("apps")
    appNames = []
    for app in apps:
        appNames.append(app.name)
    appSelected = 0

    while True:
        PhoneDisplay.clear() # TODO: get the actual time
        PhoneDisplay.drawText("12:00 PM", 42, 10, 5)
        PhoneDisplay.drawText("Monday, June 07, 2021", 25, 10, 50)
        PhoneDisplay.drawNavbar("", "Apps", "")
        PhoneDisplay.refresh()
        
        inAppMenu = False
        while True:
            key = KeyInput.getKey()
            if(key == "ok"):
                selected = PhoneDisplay.getMenuSelection(KeyInput, "Select App", appNames)
                App.launchApp(apps[selected], PhoneDisplay, KeyInput, BuildInfo, UserSettings)
                break