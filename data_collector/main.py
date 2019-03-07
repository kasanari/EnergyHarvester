import serial_interface
import threading
import time
import random
import numpy as np
import serial_utils
from node_manager import NodeManager, Node, Action
from serial_interface import ADD_NODE_HEADER, DATA_HEADER, REMOVE_NODE_HEADER, COMMIT_HEADER
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


class Plotter:
    def __init__(self, node_manager):
        self.node_manager = node_manager
        self.bar, = plt.plot(0, 0)
        self.plots = {}
        self.ani = FuncAnimation(plt.gcf(), self.run, interval=1000, repeat=True)

    def run(self, i):
        energy_levels = self.node_manager.get_energy_levels()
        for column in energy_levels.columns:
            try:
                self.plot_data(self.plots[column], energy_levels[column])
            except KeyError:
                self.plots[column], = plt.plot(0,0)
                self.plot_data(self.plots[column], energy_levels[column])

    def plot_data(self, plt, data):
        limit = -20
        plt.set_data(range(len(data[limit:])), data[limit:])
        plt.axes.relim()
        plt.axes.autoscale_view()

class DataListener(threading.Thread):
    def __init__(self, node_manager):

        threading.Thread.__init__(self)

        self.node_manager = node_manager
        self._period = 1
        self._nextCall = time.time()
        random.seed()
        self.ser = serial_utils.serial_init()

    def run(self):
        node1 = Node(1, 0)
        node2 = Node(2, 0)
        manager.add_node(node1)
        manager.add_node(node2)
        while True:
            print("Fetching data from fog node...")
            header, node = serial_interface.receive_message(self.ser)
            # header = 'data'
            # node1.energy_level = random.randint(1, 50)
            # node2.energy_level = random.randint(1,50)
            print(f'Header: {header}')
            if header == DATA_HEADER:
                print(f'\t{node}')
                self.node_manager.update_node(node)
            elif header == ADD_NODE_HEADER:
                self.node_manager.add_node(node)
            elif header == REMOVE_NODE_HEADER:
                self.node_manager.remove_node(node.node_id)
            elif header == COMMIT_HEADER:
                self.node_manager.commit(time.time())
                print(f'\tSending orders...')
                serial_interface.send_action(self.ser, 1, Action.GATHER)
                serial_interface.send_action(self.ser, 2, Action.CHARGE)
            

manager = NodeManager()
listener = DataListener(manager)
plotter = Plotter(manager)
listener.start()
plt.show()
