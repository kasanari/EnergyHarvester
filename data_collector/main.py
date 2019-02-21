import serial_interface
from node_manager import NodeManager, Node
from serial_interface import ADD_NODE_HEADER, DATA_HEADER, REMOVE_NODE_HEADER


manager = NodeManager()

while True:

    header, node = serial_interface.receive_message()
    if header == DATA_HEADER:
        pass
    elif header == ADD_NODE_HEADER:
        manager.add_node(node)
    elif header == REMOVE_NODE_HEADER:
        manager.remove_node(node.id)
