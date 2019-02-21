import serial, time
import serial_utils
from node_manager import Node

"""Node actions"""
GATHER = 1
SLEEP = 2
CHARGE = 3

"""Headers"""
ADD_NODE_HEADER = "add_node"
DATA_HEADER = "data"
REMOVE_NODE_HEADER = "remove_node"


def read_from_serial_forever():
    """Read from serial port and print output forever"""
    ser = serial_utils.serial_init()

    adc_values = []

    while True:
        line = ser.read_until()
        value = int(line[0:-1])
        print(value)
        adc_values.append(value)


def send_action(node_id, action):
    """Send an action and the id of the node to perform the action to the fog node"""
    timestamp = int(time.time())
    string_to_send = f'{node_id},{action},{timestamp}\n'
    encoded_string = string_to_send.encode('utf-8')
    ser = serial_utils.serial_init()
    ser.write(encoded_string)
    ser.close()


def receive_message():
    """Read a line from the serial port and parse it"""
    ser = serial_utils.serial_init()
    line = ser.read_until()
    ser.close()
    return parse_msg(line.decode('utf-8'))


def parse_msg(msg):
    """Parse a message, splitting it into a header part and node data part"""
    result = msg[:-1].split(',')
    header = None
    node = None
    try:
        header = result[0]
    except ValueError as error:
        print("invalid ID character")

    if header is not None:
        node = Node(result[1], result[2])

    return header, node


def parse_data(data_string):
    """Parse the data section of a message"""
    result = data_string.split(',')
    node_id = None
    energy_level = None
    try:
        node_id = int(result[0])
    except ValueError:
        print("invalid ID character")
    try:
        energy_level = float(result[1])
    except ValueError:
        print("invalid data characters")

    if (node_id is not None) and (energy_level is not None):
        return Node(node_id, energy_level)






