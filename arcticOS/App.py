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

import os, json

def launchApp(app, display, keyinput, buildsettings, usersettings):
    launchPath(app.fileName, display, keyinput, buildsettings, usersettings)

def launchPath(path, display, keyinput, buildsettings, usersettings):
    if(buildsettings.getKey("do_full_crash") == "true"):
        with open(path) as appfile:
            appdata = appfile.read()
            exec(appdata, globals())
            runApp(display, keyinput, buildsettings, usersettings)
    else:
        try:
            with open(path) as appfile:
                appdata = appfile.read()
                exec(appdata, globals())
                runApp(display, keyinput, buildsettings, usersettings)
        except:
            print("Error in app!")
            return

def getAllApps(path):
    appList = []
    for app in os.listdir(path):
        try:
            app = App(path + "/" + app + "/manifest.json")
            if(app.name != "" and app.icon != ""):
                appList.append(app)
        except:
            pass

    return appList

class App:
    def __init__(self, path):
        self.name = ""
        self.icon = ""
        self.fileName = ""
        self.loadFrom(path)

    def loadFrom(self, path): # Load app from app manifest
        try:
            with open(path) as appManifest:
                data = json.loads(appManifest.read())
                self.name = data["name"]
                self.icon = data["icon"]
                self.fileName = path[:-13] + data["fileName"]
        except:
            print("ERROR: Couldn't read app manifest: " + path)