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
    print_reply(ser)


def receive_message():
    """Read a line from the serial port and parse it"""
    ser = serial_utils.serial_init()
    line = ser.read_until()
    ser.close()
    return parse_msg(line.decode('utf-8'))

def print_reply(ser):
    reply = read_line(ser)
    print(f'\tNode received message: {reply[:-1]}')
    parsed_reply = read_line(ser)
    print(f'\tNode parsed message to: {parsed_reply[:-1]}')

def parse_msg(msg):
    if msg == '':
        return None, None

    """Parse a message, splitting it into a header part and node data part"""
    result = msg[:-1].split(',')
    header = None
    node = None
    try:
        header = result[0]
    except ValueError as error:
        print("invalid ID character")

    if header is not None:
        node = parse_data(result[1:])

    return header, node


def parse_data(data):
    """Parse the data section of a message"""
    node_id = None
    energy_level = None
    try:
        node_id = int(data[0])
    except ValueError:
        print("invalid ID character")
    try:
        energy_level = float(data[1])/10
    except ValueError:
        print("invalid data characters")

    if (node_id is not None) and (energy_level is not None):
        return Node(node_id, energy_level)






