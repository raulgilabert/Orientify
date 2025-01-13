import serial
import os

SerialObj = serial.Serial(port="/dev/ttyUSB1", baudrate=9600)

while True:
    data = SerialObj.readline()[:-2]

    if data == b'0':
        os.system("xrandr --output DisplayPort-1 --rotate left")
    elif data == b'2':
        os.system("xrandr --output DisplayPort-1 --rotate normal")


