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

# Resets all user settings to their default value. Will cause OOBE to start on next Launcher start.
def factoryReset():
    with open("settings/user_template.json") as sourcefile:
        with open("settings/user.json", "w") as destfile:
            destfile.write(sourcefile.read())

# Just a basic wrapper for dictionaries that can autowrite back to JSON.
class PreferenceStorage:
    def __init__(self, path):
        self.path = path
        self.data = {}
        self.reload()

    # Refreshes from disk.
    def reload(self):
        try:
            with open(self.path) as jsonfile:
                self.data = json.loads(jsonfile.read())
        except:
            print("ERROR: Couldn't read stored preferences: " + self.path)

    # Writes the latest data to disk.
    def write(self):
        try:
            with open(self.path, "w") as jsonfile:
                json.dump(self.data, jsonfile)
        except:
            print("ERROR: Failed to save preferences to " + self.path)

    # Gets a preference key.
    def getKey(self, key):
        if(self.hasKey(key)):
            return self.data[key]
        else:
            return None
    
    # Sets a preference key and writes the preferences to disk.
    def setKey(self, key, value):
        self.data[key] = value
        self.write()

    # Returns True if the preference file has a specified key.
    def hasKey(self, key):
        if(key in self.data):
            return True
        return False

    # Returns True if there is a value in the key.
    def doesKeyHaveValue(self, key):
        if(key in self.data):
            if(key != ""):
                return True
        return False