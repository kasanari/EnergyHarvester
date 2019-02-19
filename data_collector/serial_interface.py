import serial, time
import serial_utils

def read_from_serial_forever():
    ser = serial_init()

    adc_values = []

    while(1):
        line = ser.read_until()
        value = int(line[0:-1])
        print(value)
    adc_values.append(value)
    ser.close()

def send_action(node_id, action):
    timestamp = int(time.time())
    string_to_send = f'{node_id},{action},{timestamp}\n'
    encoded_string = string_to_send.encode('utf-8')
    ser = serial_init()
    ser.write(encoded_string)
    ser.close()

def receive_data():
    ser = serial_init()
    line = ser.read_until()
    ser.close()
    return line





