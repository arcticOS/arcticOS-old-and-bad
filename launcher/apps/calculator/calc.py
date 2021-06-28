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

import math

def runApp(PhoneDisplay, KeyInput, BuildInfo, UserSettings):
    operations = ["+", "-", "*", "/"]
    operation = 0
    number0 = "0"
    number1 = "0"
    total = ""
    currentNumber = 0
    PhoneDisplay.clear()
    PhoneDisplay.drawAppHeader("Calculator")
    PhoneDisplay.drawNavbar("Exit", "Change Operation", operations[operation])
    PhoneDisplay.drawText("0", 25, 10, 35)
    PhoneDisplay.refresh()

    while True:
        key = KeyInput.getKey()

        if(key == "end"):
            break
        elif(key == "ok"):
            operation += 1
            if(operation > 3):
                operation = 0
        elif(key == "answer"):
            if(currentNumber == 0):
                currentNumber = 1
            else:
                currentNumber = 2
                if(operation == 0):
                    total = str(float(number0) + float(number1))
                elif(operation == 1):
                    total = str(float(number0) - float(number1))
                elif(operation == 2):
                    total = str(float(number0) * float(number1))
                elif(operation == 3):
                    total = str(float(number0) / float(number1))
                if(float(total) % math.floor(float(total)) == 0):
                    total = str(math.floor(float(total)))
        else:
            if(key.isdigit()):
                if(currentNumber == 0):
                    if(number0 == "0"):
                        number0 = key
                    else:
                        number0 = number0 + key
                elif(currentNumber == 1):
                    if(number1 == "0"):
                        number1 = key
                    else:
                        number1 = number1 + key
                else:
                    number0 = key
                    number1 = "0"
                    currentNumber = 0
                    operation = 0
            if(key == "."):
                if(currentNumber == 0):
                    number0 = number0 + key
                elif(currentNumber == 1):
                    number1 = number1 + key
                else:
                    number0 = "0" + key
                    number1 = "0"
                    currentNumber = 0
                    operation = 0
    
        PhoneDisplay.drawAppHeader("Calculator")
        if(currentNumber == 0):
            PhoneDisplay.drawNavbar("Exit", "Change Operation", operations[operation])
        else:
            PhoneDisplay.drawNavbar("Exit", "Change Operation", "=")

        if(currentNumber > 0):
            PhoneDisplay.drawText(number0 + " " + operations[operation], 25, 10, 35)
            PhoneDisplay.drawText(number1, 25, 10, 60)
        else:
            PhoneDisplay.drawText(number0, 25, 10, 35)

        if(currentNumber == 2):
            PhoneDisplay.drawLine(10, 90, 60, 90)
            PhoneDisplay.drawText(total, 25, 10, 90)
        PhoneDisplay.refresh()