import serial_interface
import threading
from node_manager import NodeManager, Node
from serial_interface import ADD_NODE_HEADER, DATA_HEADER, REMOVE_NODE_HEADER


manager = NodeManager()

class DataListener(threading.Thread):
    def __init__(self, node_manager):

        threading.Thread.__init__(self)

        self._node_manager = node_manager

        def run(self):
            while True:
                print("Fetching data from fog node...")
                header, node = serial_interface.receive_message()
                if header == DATA_HEADER:
                    pass
                elif header == ADD_NODE_HEADER:
                    manager.add_node(node)
                elif header == REMOVE_NODE_HEADER:
                    manager.remove_node(node.id)

while True:
    listener = DataListener(manager)
    listener.run()
