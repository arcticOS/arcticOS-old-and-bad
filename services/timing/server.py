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

# NOTE: using http.server here isn't a huge issue. the phone has no wifi and doesn't expose ports over cellular.
from http.server import BaseHTTPRequestHandler, HTTPServer
import os, sys, time, base64

global hostName, serverPort

hostName = "localhost"
serverPort = 3504

class WatchdogService(BaseHTTPRequestHandler):
    def do_GET(self):
        global driver

        if(self.path.startswith("/display/")):
            self.send_response(200)
            self.end_headers()
            path = self.path[9:]
            path = base64.b64decode(path)
            path = path.decode("ascii").split("\r")
            print("INFO: WatchdogService: Request: " + str(path))
                
            
        else:
            self.send_response(404)
            self.end_headers()

if(__name__ == "__main__"):
    webserver = HTTPServer((hostName, serverPort), WatchdogService)
    print("INFO: WatchdogService: Started.")

    try:
        webserver.serve_forever()
    except KeyboardInterrupt:
        pass

    webserver.server_close()
    print("INFO: WatchdogService: Stopped.")