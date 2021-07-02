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
import pygame, pygame.font
import os, sys, time, base64, threading

global hostName, serverPort

hostName = "localhost"
serverPort = 3503

# This mainly just wraps Pygame functions into a DisplayDriver.
class DisplayDriver(object):
    def __init__(self):
        self.color_black = (0, 0, 0)
        self.color_white = (255, 255, 255)
        self.color_blue = (0, 0, 255)

        pygame.init()
        self.display = pygame.display.set_mode((350, 250))
        pygame.display.set_caption("arcticOS Emulator Input")
        pygame.font.init()

        self.font = pygame.font.SysFont("Sans Serif", 16) # TODO: Use a standard font

        self.width = 264
        self.height = 176

    def clear(self):
        self.display.fill(self.color_white)

    def refresh(self):
        for event in pygame.event.get():
            if(event.type == pygame.QUIT):
                print("EMULATOR DEBUG: Exit requested!")
                pygame.quit()
                exit(0)

        pygame.display.update()
        self.clear()

    def getTextBounds(self, text, size):
        bounds = self.font.size(text)
        return bounds

    def drawText(self, text, size, x, y):
        rendered_text = self.font.render(text, True, self.color_black)
        self.display.blit(rendered_text, (x, y))

    def drawTextCentered(self, text, x, y):
        bounds = self.font.size(text)
        x = x - (bounds[0] / 2)
        y = y - (bounds[1] / 2)
        rendered_text = self.font.render(text, True, self.color_black)
        self.display.blit(rendered_text, (x, y))

    def drawRect(self, x, y, width, height):
        pygame.draw.rect(self.display, self.color_black, pygame.Rect(x, y, width, height), width = 1)

    def fillRect(self, x, y, width, height):
        pygame.draw.rect(self.display, self.color_black, pygame.Rect(x, y, width, height))

    def drawLine(self, x1, y1, x2, y2):
        pygame.draw.line(self.display, self.color_black, (x1, y1), (x2, y2))
global driver
driver = DisplayDriver()

class InputService(BaseHTTPRequestHandler):
    def do_GET(self):
        global driver, buttonsPressed

        if(self.path.startswith("/input/")):
            self.send_response(200)
            self.end_headers()
            path = self.path[7:]
            path = base64.b64decode(path)
            path = path.decode("ascii").split("\r")
            print("INFO: InputService: Request: " + str(path))
            if(path[0] == "G"): # Get keys pressed
                for button in buttonsPressed:
                    if(buttonsPressed[button]):
                        self.wfile.write(bytes(button + ",", "utf-8"))
            elif(path[0] == "S"):
                for button in buttonsPressed:
                    if(buttonsPressed[button]):
                        self.wfile.write(bytes(button, "utf-8"))
                        break
            
            for button in buttonsPressed:
                buttonsPressed[button] = False
            
        else:
            self.send_response(404)
            self.end_headers()

class HTTPServerThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.webserver = HTTPServer((hostName, serverPort), InputService)
    def run(self):
        try:
            self.webserver.serve_forever()
        except:
            self.webserver.server_close()

def draw_button(text, x, y, w=20, h=30):
    global driver
    driver.drawRect(x, y, w, h)
    driver.drawTextCentered(text, x + (w / 2), y + (h / 2))
    pos = pygame.mouse.get_pos()
    if(pos[0] > x and pos[0] < x + w):
        if(pos[1] > y and pos[1] < y + h):
            if(pygame.mouse.get_pressed()[0]):
                driver.fillRect(x, y, w, h)
                return True
            else:
                driver.drawRect(x + 1, y + 1, w - 2, h - 2)
    return False

def getNormalButtons():
    buttonsPushed = {}
    buttonsPushed["Q"] = draw_button("Q", 10, 10)
    buttonsPushed["W"] = draw_button("W", 35, 10)
    buttonsPushed["E"] = draw_button("E", 60, 10)
    buttonsPushed["R"] = draw_button("R", 85, 10)
    buttonsPushed["T"] = draw_button("T", 110, 10)
    buttonsPushed["Y"] = draw_button("Y", 135, 10)
    buttonsPushed["U"] = draw_button("U", 160, 10)
    buttonsPushed["I"] = draw_button("I", 185, 10)
    buttonsPushed["O"] = draw_button("O", 210, 10)
    buttonsPushed["P"] = draw_button("P", 235, 10)

    buttonsPushed["A"] = draw_button("A", 10, 45)
    buttonsPushed["S"] = draw_button("S", 35, 45)
    buttonsPushed["D"] = draw_button("D", 60, 45)
    buttonsPushed["F"] = draw_button("F", 85, 45)
    buttonsPushed["G"] = draw_button("G", 110, 45)
    buttonsPushed["H"] = draw_button("H", 135, 45)
    buttonsPushed["J"] = draw_button("J", 160, 45)
    buttonsPushed["K"] = draw_button("K", 185, 45)
    buttonsPushed["L"] = draw_button("L", 210, 45)
    buttonsPushed["back"] = draw_button("Back", 235, 45, w=30)

    buttonsPushed["Alt"] = draw_button("Alt", 10, 80, w=25)
    buttonsPushed["Z"] = draw_button("Z", 40, 80)
    buttonsPushed["X"] = draw_button("X", 65, 80)
    buttonsPushed["C"] = draw_button("C", 90, 80)
    buttonsPushed["V"] = draw_button("V", 115, 80)
    buttonsPushed["B"] = draw_button("B", 140, 80)
    buttonsPushed["N"] = draw_button("N", 165, 80)
    buttonsPushed["M"] = draw_button("M", 190, 80)
    buttonsPushed["$"] = draw_button("$", 215, 80)
    buttonsPushed["enter"] = draw_button("Ret", 240, 80, w=25)

    buttonsPushed["Shift"] = draw_button("Shift", 10, 115, w=30)
    buttonsPushed["special"] = draw_button("Special", 45, 115, w=40)
    buttonsPushed[" "] = draw_button("Space", 90, 115, w=80)
    buttonsPushed["sym"] = draw_button("Sym", 175, 115, w=30)
    if not(buttonsPushed["Shift"]):
        buttonsPushed["Shift"] = draw_button("Shift", 210, 115, w=30)
    else:
        draw_button("Shift", 210, 115, w=30)

    buttonsPushed["left"] = draw_button("<", 10, 150, w=30)
    buttonsPushed["up"] = draw_button("/\\", 45, 150, w=30)
    buttonsPushed["ok"] = draw_button("Ok", 80, 150, w=30)
    buttonsPushed["down"] = draw_button("\\/", 115, 150, w=30)
    buttonsPushed["right"] = draw_button(">", 150, 150, w=30)

    buttonsPushed["answer"] = draw_button("Ans", 150, 185, w=30)
    buttonsPushed["lUtil"] = draw_button("-", 45, 185, w=30)
    buttonsPushed["rUtil"] = draw_button("-", 115, 185, w=30)
    buttonsPushed["end"] = draw_button("End", 10, 185, w=30)

    buttonsPushed["vUp"] = draw_button("Vol+", 300, 10, w=30)
    buttonsPushed["vDown"] = draw_button("Vol-", 300, 45, w=30)
    buttonsPushed["light"] = draw_button("Light", 300, 80, w=35)

    return buttonsPushed

def getSpecialButtons():
    buttonsPushed = {}
    buttonsPushed["#"] = draw_button("#", 10, 10)
    buttonsPushed["1"] = draw_button("1", 35, 10)
    buttonsPushed["2"] = draw_button("2", 60, 10)
    buttonsPushed["3"] = draw_button("3", 85, 10)
    buttonsPushed["("] = draw_button("(", 110, 10)
    buttonsPushed[")"] = draw_button(")", 135, 10)
    buttonsPushed["_"] = draw_button("_", 160, 10)
    buttonsPushed["-"] = draw_button("-", 185, 10)
    buttonsPushed["+"] = draw_button("+", 210, 10)
    buttonsPushed["@"] = draw_button("@", 235, 10)

    buttonsPushed["*"] = draw_button("*", 10, 45)
    buttonsPushed["4"] = draw_button("4", 35, 45)
    buttonsPushed["5"] = draw_button("5", 60, 45)
    buttonsPushed["6"] = draw_button("6", 85, 45)
    buttonsPushed["/"] = draw_button("/", 110, 45)
    buttonsPushed[":"] = draw_button(":", 135, 45)
    buttonsPushed[";"] = draw_button(";", 160, 45)
    buttonsPushed["'"] = draw_button("'", 185, 45)
    buttonsPushed["\""] = draw_button("\"", 210, 45)

    buttonsPushed["7"] = draw_button("7", 40, 80)
    buttonsPushed["8"] = draw_button("8", 65, 80)
    buttonsPushed["9"] = draw_button("9", 90, 80)
    buttonsPushed["?"] = draw_button("?", 115, 80)
    buttonsPushed["!"] = draw_button("!", 140, 80)
    buttonsPushed[","] = draw_button(",", 165, 80)
    buttonsPushed["."] = draw_button(".", 190, 80)

    buttonsPushed["0"] = draw_button("0", 45, 115, w=40)
    buttonsPushed["sym"] = draw_button("Sym", 175, 115, w=30)

    buttonsPushed["left"] = draw_button("<", 10, 150, w=30)
    buttonsPushed["up"] = draw_button("/\\", 45, 150, w=30)
    buttonsPushed["ok"] = draw_button("Ok", 80, 150, w=30)
    buttonsPushed["down"] = draw_button("\\/", 115, 150, w=30)
    buttonsPushed["right"] = draw_button(">", 150, 150, w=30)

    buttonsPushed["answer"] = draw_button("Ans", 150, 185, w=30)
    buttonsPushed["lUtil"] = draw_button("-", 45, 185, w=30)
    buttonsPushed["rUtil"] = draw_button("-", 115, 185, w=30)
    buttonsPushed["end"] = draw_button("End", 10, 185, w=30)

    buttonsPushed["vUp"] = draw_button("Vol+", 300, 10, w=30)
    buttonsPushed["vDown"] = draw_button("Vol-", 300, 45, w=30)
    buttonsPushed["light"] = draw_button("Light", 300, 80, w=35)

    return buttonsPushed

global buttonsPressed
if(__name__ == "__main__"):
    print("INFO: WatchdogService: Started.")

    httpServerThread = HTTPServerThread()
    httpServerThread.start()

    try:
        buttonSet = 0
        shift = 0
        while True:
            driver.clear()

            if(buttonSet == 0):
                buttonsPressed = getNormalButtons()
            else:
                buttonsPressed = getSpecialButtons()

            if(buttonsPressed["sym"]):
                if(buttonSet == 0):
                    buttonSet = 1
                else:
                    buttonSet = 0
            
            if(buttonsPressed["Shift"]):
                if(shift == 0):
                    shift = 1
                else:
                    shift = 0
            
            if(shift):
                newButtonsPressed = buttonsPressed
                for button in buttonsPressed:
                    if(button in "ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
                        newButtonsPressed[button.toLowerCase()] = buttonsPressed[button]
                        newButtonsPressed.pop(button)
                buttonsPressed = newButtonsPressed

            while(pygame.mouse.get_pressed()[0]):
                for event in pygame.event.get():
                    if(event.type == pygame.QUIT):
                        print("EMULATOR DEBUG: Exit requested!")
                        pygame.quit()
                        exit(0)

            driver.refresh()
    except KeyboardInterrupt:
        print("Shutdown requested.")

    print("INFO: WatchdogService: Stopped.")