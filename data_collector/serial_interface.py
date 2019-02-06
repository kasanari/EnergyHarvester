import serial, glob, os

import serial_utils

#import matplotlib.pyplot as plt

ser = serial.Serial(timeout=2)
ser.baudrate = 115200

comPort = serial_utils.findPort()
print(comPort)
ser.port = comPort

ser.open()

time_scale = range(1,)
adc_values = []

while(1):
    line = ser.read_until()
    value = int(line[0:-1])
    print(value)
    adc_values.append(value)
ser.close()


