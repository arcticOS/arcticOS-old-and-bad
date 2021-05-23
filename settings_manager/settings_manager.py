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
#
# This is just a small application to read/reset user settings and build info
# TODO: Update paths to include final settings paths

import sys, json

try:
    operation = sys.argv[1] # set/read
    settings_type = sys.argv[2] # user/build
    value = sys.argv[3] # settings item

    # Make sure above values are valid
    if(operation not in ["set", "read"]):
        exit(-2) # Operation not valid

    if(settings_type == "user"):
        data = {}
        with open("user_settings.json") as jsonfile:
            data = json.loads(jsonfile.read())

        if(operation == "set"):
            setvalue = sys.argv[4]
            data[value] = setvalue
            with open("user_settings.json", "w") as jsonfile:
                json.dump(data, jsonfile)
        else:
            print(data[value])
    else:
        data = {}
        with open("build_settings.json") as jsonfile:
            data = json.loads(jsonfile.read())

        print(data[value])
except:
    exit(-1) # Unknown error