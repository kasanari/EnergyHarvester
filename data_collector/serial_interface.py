import serial, time
import serial_utils
from node_manager import Node

"""Node actions"""
CHARGE = 1
SLEEP = 2
GATHER = 3
INVALID = 4

"""Headers"""
ADD_NODE_HEADER = "add_node"
DATA_HEADER = "data"
REMOVE_NODE_HEADER = "remove_node"
COMMIT_HEADER = "commit"


def read_from_serial_forever():
    """Read from serial port and print output forever"""
    ser = serial_utils.serial_init()

    while True:
        header, node = receive_message(ser)
        print(header)
        print(node)


def send_action(ser, node_id, action):
    """Send an action and the id of the node to perform the action to the fog node"""
    timestamp = int(time.time())
    string = f'{node_id},{action},{timestamp}\n'
    send_string(ser, string)

def send_string(ser, string):
    ser.write(string.encode('utf-8'))
    print_reply(ser)

def read_line(ser):
    return ser.read_until().decode('utf-8')

def receive_message(ser):
    """Read a line from the serial port and parse it"""
    return parse_msg(read_line(ser))

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
