import serial
import os
import glob


def find_port():
    com_port = None
    if com_port is None and os.environ.get("GOODFET") is not None:
        glob_list = glob.glob(os.environ.get("GOODFET"))
        if len(glob_list) > 0:
            com_port = glob_list[0]
    if com_port is None:
        glob_list = glob.glob("/dev/tty.usbserial*")
        if len(glob_list) > 0:
            com_port = glob_list[0]
    if com_port is None:
        glob_list = glob.glob("/dev/ttyUSB*")
        if len(glob_list) > 0:
            com_port = glob_list[0]
    return com_port


def serial_init():
    ser = serial.Serial(timeout=2)
    ser.baudrate = 115200
    com_port = find_port()
    ser.port = com_port
    ser.open()
    #print(f'Initialized serial using port {com_port}')
    return ser
