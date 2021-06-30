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
# This software contains code from Waveshare, released under the following license:
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# NOTE: using http.server here isn't a huge issue. the phone has no wifi and doesn't expose ports over cellular.
from http.server import BaseHTTPRequestHandler, HTTPServer
import os, sys, time, base64

global hostName, serverPort, driver

hostName = "localhost"
serverPort = 3502
if os.path.exists('/sys/bus/platform/drivers/gpiomem-bcm2835'): # Check if we're running on an RPI
    from PIL import Image,ImageDraw,ImageFont

    # This driver was written by Waveshare and modified to work with arcticOS.
    # I don't know what any of the display commands do, so you're on your own here.
    class DisplayDriver(object):
        def __init__(self):
            import spidev
            import RPi.GPIO

            self.reset_pin = 17
            self.dc_pin = 25
            self.busy_pin = 24
            self.cs_pin = 8
            self.width = 176
            self.height = 264
            self.WHITE  = 0xff
            self.GRAY1  = 0xC0
            self.GRAY2  = 0x80
            self.BLACK  = 0x00

            self.width = 264
            self.height = 176

            self.GPIO = RPi.GPIO
            self.SPI = spidev.SpiDev()

            if (self.module_init() != 0):
                print("ERROR: Couldn't initialise display!")
                sys.exit(-1)
                
            # EPD hardware init start
            self.reset()
            
            self.send_command(0x01) # POWER_SETTING
            self.send_data(0x03) # VDS_EN, VDG_EN
            self.send_data(0x00) # VCOM_HV, VGHL_LV[1], VGHL_LV[0]
            self.send_data(0x2b) # VDH
            self.send_data(0x2b) # VDL
            self.send_data(0x09) # VDHR
            
            self.send_command(0x06) # BOOSTER_SOFT_START
            self.send_data(0x07)
            self.send_data(0x07)
            self.send_data(0x17)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0x60)
            self.send_data(0xA5)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0x89)
            self.send_data(0xA5)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0x90)
            self.send_data(0x00)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0x93)
            self.send_data(0x2A)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0xA0)
            self.send_data(0xA5)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0xA1)
            self.send_data(0x00)
            
            # Power optimization
            self.send_command(0xF8)
            self.send_data(0x73)
            self.send_data(0x41)
            
            self.send_command(0x16) # PARTIAL_DISPLAY_REFRESH
            self.send_data(0x00)
            self.send_command(0x04) # POWER_ON
            self.ReadBusy()

            self.send_command(0x00) # PANEL_SETTING
            self.send_data(0xAF) # KW-BF   KWR-AF    BWROTP 0f
            
            self.send_command(0x30) # PLL_CONTROL
            self.send_data(0x3A) # 3A 100HZ   29 150Hz 39 200HZ    31 171HZ
        
            self.send_command(0X50)			#VCOM AND DATA INTERVAL SETTING			
            self.send_data(0x57)
            
            self.send_command(0x82) # VCM_DC_SETTING_REGISTER
            self.send_data(0x12)
            self.set_lut()

            self.Clear(0xFF)
            self.buffer = Image.new('1', (176, 264), 255)
            self.draw = ImageDraw.Draw(self.buffer)

        lut_vcom_dc = [0x00, 0x00,
            0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
            0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
            0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
            0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        ]
        lut_ww = [
            0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
            0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
            0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
            0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        lut_bw = [
            0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
            0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
            0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
            0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        lut_bb = [
            0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
            0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
            0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
            0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        lut_wb = [
            0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
            0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
            0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
            0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        ###################full screen update LUT######################
        #0~3 gray
        gray_lut_vcom = [
        0x00, 0x00,
        0x00, 0x0A, 0x00, 0x00, 0x00, 0x01,
        0x60, 0x14, 0x14, 0x00, 0x00, 0x01,
        0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x13, 0x0A, 0x01, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,				
        ]
        #R21
        gray_lut_ww =[
        0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
        0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
        0x10, 0x14, 0x0A, 0x00, 0x00, 0x01,
        0xA0, 0x13, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        #R22H	r
        gray_lut_bw =[
        0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
        0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
        0x00, 0x14, 0x0A, 0x00, 0x00, 0x01,
        0x99, 0x0C, 0x01, 0x03, 0x04, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        #R23H	w
        gray_lut_wb =[
        0x40, 0x0A, 0x00, 0x00, 0x00, 0x01,
        0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
        0x00, 0x14, 0x0A, 0x00, 0x00, 0x01,
        0x99, 0x0B, 0x04, 0x04, 0x01, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]
        #R24H	b
        gray_lut_bb =[
        0x80, 0x0A, 0x00, 0x00, 0x00, 0x01,
        0x90, 0x14, 0x14, 0x00, 0x00, 0x01,
        0x20, 0x14, 0x0A, 0x00, 0x00, 0x01,
        0x50, 0x13, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ]

        def digital_write(self, pin, value):
            self.GPIO.output(pin, value)

        def digital_read(self, pin):
            return self.GPIO.input(pin)

        def delay_ms(self, delaytime):
            time.sleep(delaytime / 1000.0)

        def spi_writebyte(self, data):
            self.SPI.writebytes(data)

        def spi_writebyte2(self, data):
            self.SPI.writebytes2(data)

        def module_init(self):
            self.GPIO.setmode(self.GPIO.BCM)
            self.GPIO.setwarnings(False)
            self.GPIO.setup(self.reset_pin, self.GPIO.OUT)
            self.GPIO.setup(self.dc_pin, self.GPIO.OUT)
            self.GPIO.setup(self.cs_pin, self.GPIO.OUT)
            self.GPIO.setup(self.busy_pin, self.GPIO.IN)

            # SPI device, bus = 0, device = 0
            self.SPI.open(0, 0)
            self.SPI.max_speed_hz = 4000000
            self.SPI.mode = 0b00
            return 0

        def module_exit(self):
            self.SPI.close()
            self.GPIO.output(self.reset_pin, 0)
            self.GPIO.output(self.dc_pin, 0)

            self.GPIO.cleanup()
        
        def reset(self):
            self.digital_write(self.reset_pin, 1)
            self.delay_ms(200) 
            self.digital_write(self.reset_pin, 0)
            self.delay_ms(5)
            self.digital_write(self.reset_pin, 1)
            self.delay_ms(200)   

        def send_command(self, command):
            self.digital_write(self.dc_pin, 0)
            self.digital_write(self.cs_pin, 0)
            self.spi_writebyte([command])
            self.digital_write(self.cs_pin, 1)

        def send_data(self, data):
            self.digital_write(self.dc_pin, 1)
            self.digital_write(self.cs_pin, 0)
            self.spi_writebyte([data])
            self.digital_write(self.cs_pin, 1)
            
        def ReadBusy(self):        
            while(self.digital_read(self.busy_pin) == 0):      #  0: idle, 1: busy
                self.delay_ms(200)                

        def set_lut(self):
            self.send_command(0x20) # vcom
            for count in range(0, 44):
                self.send_data(self.lut_vcom_dc[count])
            self.send_command(0x21) # ww --
            for count in range(0, 42):
                self.send_data(self.lut_ww[count])
            self.send_command(0x22) # bw r
            for count in range(0, 42):
                self.send_data(self.lut_bw[count])
            self.send_command(0x23) # wb w
            for count in range(0, 42):
                self.send_data(self.lut_bb[count])
            self.send_command(0x24) # bb b
            for count in range(0, 42):
                self.send_data(self.lut_wb[count])
                
        def gray_SetLut(self):
            self.send_command(0x20)
            for count in range(0, 44):        #vcom
                self.send_data(self.gray_lut_vcom[count])
                
            self.send_command(0x21)							#red not use
            for count in range(0, 42): 
                self.send_data(self.gray_lut_ww[count])

            self.send_command(0x22)							#bw r
            for count in range(0, 42): 
                self.send_data(self.gray_lut_bw[count])

            self.send_command(0x23)							#wb w
            for count in range(0, 42): 
                self.send_data(self.gray_lut_wb[count])

            self.send_command(0x24)							#bb b
            for count in range(0, 42): 
                self.send_data(self.gray_lut_bb[count])

            self.send_command(0x25)							#vcom
            for count in range(0, 42): 
                self.send_data(self.gray_lut_ww[count])

        def getbuffer(self, image):
            # logging.debug("bufsiz = ",int(self.width/8) * self.height)
            buf = [0xFF] * (int(self.width/8) * self.height)
            image_monocolor = image.convert('1')
            imwidth, imheight = image_monocolor.size
            pixels = image_monocolor.load()
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
            return buf
        
        def display(self, image):
            self.send_command(0x10)
            for i in range(0, int(self.width * self.height / 8)):
                self.send_data(0xFF)
            self.send_command(0x13)
            for i in range(0, int(self.width * self.height / 8)):
                self.send_data(image[i])
            self.send_command(0x12) 
            self.ReadBusy()
            
        def Clear(self, color=0xFF):
            self.send_command(0x10)
            for i in range(0, int(self.width * self.height / 8)):
                self.send_data(color)
            self.send_command(0x13)
            for i in range(0, int(self.width * self.height / 8)):
                self.send_data(color)
            self.send_command(0x12) 
            self.ReadBusy()

        def sleep(self):
            self.send_command(0X50)
            self.send_data(0xf7)
            self.send_command(0X02)
            self.send_command(0X07)
            self.send_data(0xA5)
            
            self.delay_ms(2000)
            self.module_exit()

        def clear(self):
            self.draw.rectangle([0, 0, 176, 264], fill=(255, 255, 255, 255))

        def refresh(self):
            self.display(self.getbuffer(self.buffer))
            self.clear()

        def getTextBounds(self, text, size):
            font = ImageFont.truetype("font.ttf", size)
            w, h = self.draw.textsize(text, font=font)
            return (w, h)

        def drawText(self, text, size, x, y):
            font = ImageFont.truetype("font.ttf", size)
            self.draw.text((x, y), text)

        def drawTextCentered(self, text, size, x, y):
            font = ImageFont.truetype("font.ttf", size)
            w, h = self.draw.textsize(text, font=font)
            self.draw.text((x - (w/2), y - (h/2)), text, font=font)
        
        def drawRect(self, x, y, width, height):
            self.draw.rectangle([x, y, x + width, y + height], outline=0x00, width=1)

        def fillRect(self, x, y, width, height):
            self.draw.rectangle([x, y, x + width, y + height], fill=0xFF)

        def drawLine(self, x1, y1, x2, y2):
            self.draw.line([x1, y1, x2, y2], fill=0x00)
else:
    import pygame
    import pygame.font

    # This mainly just wraps Pygame functions into a DisplayDriver.
    class DisplayDriver(object):
        def __init__(self):
            self.color_black = (0, 0, 0)
            self.color_white = (255, 255, 255)

            pygame.init()
            self.display = pygame.display.set_mode((264, 176))
            pygame.display.set_caption("arcticOS Emulator")
            pygame.font.init()

            self.font_small_normal = pygame.font.SysFont("Sans Serif", 16) # TODO: Use a standard font

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
            font = pygame.font.Font("arcticOS/font.ttf", size)
            bounds = font.size(text)
            return bounds

        def drawText(self, text, size, x, y):
            font = pygame.font.Font("arcticOS/font.ttf", size)
            rendered_text = font.render(text, True, self.color_black)
            self.display.blit(rendered_text, (x, y))

        def drawTextCentered(self, text, size, x, y):
            font = pygame.font.Font("arcticOS/font.ttf", size)
            bounds = font.size(text)
            x = x - (bounds[0] / 2)
            y = y - (bounds[1] / 2)
            rendered_text = font.render(text, True, self.color_black)
            self.display.blit(rendered_text, (x, y))

        def drawRect(self, x, y, width, height):
            pygame.draw.rect(self.display, self.color_black, pygame.Rect(x, y, width, height), width = 1)

        def fillRect(self, x, y, width, height):
            pygame.draw.rect(self.display, self.color_black, pygame.Rect(x, y, width, height))

        def drawLine(self, x1, y1, x2, y2):
            pygame.draw.line(self.display, self.color_black, (x1, y1), (x2, y2))
driver = DisplayDriver()

class DisplayServer(BaseHTTPRequestHandler):
    def do_GET(self):
        global driver

        if(self.path.startswith("/display/")):
            self.send_response(200)
            self.end_headers()
            path = self.path[9:]
            path = base64.b64decode(path)
            path = path.decode("ascii").split("\r")
            print("INFO: DisplayService: Request: " + str(path))
                
            if(path[0] == "C"): # Clear
                driver.clear()
            elif(path[0] == "R"): # Refresh
                driver.refresh()
            elif(path[0] == "B"): # Bounds
                bounds = driver.getTextBounds(path[1], int(float(path[2])))
                self.wfile.write(bytes(str(bounds[0]) + "," + str(bounds[1]) + "\n", "utf-8"))
            elif(path[0] == "T"): # Text (Normal)
                driver.drawText(path[1], int(float(path[2])), int(float(path[3])), int(float(path[4])))
            elif(path[0] == "TC"): # Text (Centered)
                driver.drawTextCentered(path[1], int(float(path[2])), int(float(path[3])), int(float(path[4])))
            elif(path[0] == "R"): # Rectangle (Normal)
                driver.drawRect(int(float(path[1])), int(float(path[2])), int(float(path[3])), int(float(path[4])))
            elif(path[0] == "RF"): # Rectangle (Filled)
                driver.fillRect(int(float(path[1])), int(float(path[2])), int(float(path[3])), int(float(path[4])))
            elif(path[0] == "L"): # Line
                driver.drawLine(int(float(path[1])), int(float(path[2])), int(float(path[3])), int(float(path[4])))
        else:
            self.send_response(404)
            self.end_headers()

if(__name__ == "__main__"):
    webserver = HTTPServer((hostName, serverPort), DisplayServer)
    print("INFO: DisplayService: Started.")

    try:
        webserver.serve_forever()
    except KeyboardInterrupt:
        pass

    webserver.server_close()
    print("INFO: DisplayService: Stopped.")