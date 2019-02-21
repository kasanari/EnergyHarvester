import serial, time
import serial_utils

"""Node actions"""
GATHER = 1
SLEEP = 2
CHARGE = 3

"""Headers"""
ADD_NODE_HEADER = "add_node"
DATA_HEADER = "data"
REMOVE_NODE_HEADER = "remove_node"

def read_from_serial_forever():
    ser = serial_utils.serial_init()

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

def receive_message():
    ser = serial_init()
    line = ser.read_until()
    ser.close()
    return parse_msg(line)

def parse_msg(msg):
    result = msg.split(',')
    header = None
    try:
        header = result[0]
    except ValueError as error:
        print("invalid ID character")

    if header is not None:
        node = parse_data(result[1:])

    return header, node


def parse_data(data_string):
    result = data_string.split(',')
    node_id = None
    energy_level = None
    try:
        node_id = int(result[0])
    except ValueError as error:
        print("invalid ID character")
    try:
        energy_level = float(result[1])
    except ValueError as error:
        print("invalid data characters")

    if ((node_id is not None) and (energy_level is not None)):
        return Node(node_id, energy_level)






