import random
import node_manager
import serial_interface
from serial_interface import GATHER, CHARGE

random.seed()

def fake_data_generator():
    id = random.randint(0, 3)
    energy_level = random.randint(0, 5)
    return f"{id},{energy_level}"


def make_decision(node):
    if (node.energy_level > 2.5):
        send_action(node.id, GATHER)
    elif (node.energy_level < 2.1):
        send_action(node.id, CHARGE)
