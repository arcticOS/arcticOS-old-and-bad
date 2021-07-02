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

from arcticOS import HID, App, PreferenceStorage

global BuildInfo, UserSettings
# BuildInfo stores information that the user shouldn't normally need to change
BuildInfo = PreferenceStorage.PreferenceStorage("settings/build.json")

# The always_factory_reset key should be disabled in any production versions
if(BuildInfo.getKey("always_factory_reset")):
    PreferenceStorage.factoryReset()

# UserSettings stores anything that the user should normally need to change
UserSettings = PreferenceStorage.PreferenceStorage("settings/user.json")

# The HID file handles drivers for the keyboard, screen, and (soon) sound
global PhoneDisplay, KeyInput
PhoneDisplay = HID.Display()
KeyInput = HID.KeyInput()

if(__name__ == "__main__"):
    # The out of box experience should be the initial setup process for the phone
    # (Name, date/time, etc...)
    # We use the OOBEComplete UserSettings key to determine whether or not this
    # actually needs to be run
    if(UserSettings.getKey("OOBEComplete") == 0):
        print("Launching OOBE...")
        App.launchPath("apps/system/oobe.py", PhoneDisplay, KeyInput, BuildInfo, UserSettings)

        UserSettings.setKey("OOBEComplete", 1)

    # We need to get a list of all apps, and then make another list to store
    # just the names. We do this because getMenuSelection() only works with
    # strings.
    apps = App.getAllApps("apps")
    appNames = []
    for app in apps:
        appNames.append(app.name)
    appSelected = 0

    # Very basic main loop with an app selector.
    while True:
        PhoneDisplay.clear()
        # Show the time
        # TODO: Get the time instead of using placeholder time
        PhoneDisplay.drawText("12:00 PM", 42, 10, 5)
        PhoneDisplay.drawText("Monday, June 07, 2021", 25, 10, 50)

        # Draw the rest of the UI
        PhoneDisplay.drawNavbar("", "Apps", "")
        PhoneDisplay.refresh()
        
        # Wait for a key press
        while True:
            key = KeyInput.getKey()
            if(key == "ok"):
                # Get and start an app with a menu
                selected = PhoneDisplay.getMenuSelection(KeyInput, "Select App", appNames)
                App.launchApp(apps[selected], PhoneDisplay, KeyInput, BuildInfo, UserSettings)
                break