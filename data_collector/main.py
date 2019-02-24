import serial_interface
import threading
import time
import random
import numpy as np
from node_manager import NodeManager, Node
from serial_interface import ADD_NODE_HEADER, DATA_HEADER, REMOVE_NODE_HEADER
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

    def run(self):
        node1 = Node(1, 0)
        node2 = Node(2, 0)
        manager.add_node(node1)
        manager.add_node(node2)
        while True:
            print("Fetching data from fog node...")
            # header, node = serial_interface.receive_message()
            header = 'data'
            node1.energy_level = random.randint(1, 50)
            node2.energy_level = random.randint(1,50)
            timestamp = time.process_time()
            print(f'Header: {header}')
            if header == DATA_HEADER:
                self.node_manager.update_node(node1)
                self.node_manager.update_node(node2)
            elif header == ADD_NODE_HEADER:
                self.node_manager.add_node(node)
            elif header == REMOVE_NODE_HEADER:
                self.node_manager.remove_node(node.node_id)

            self.node_manager.commit(time.time())
            self._nextCall = self._nextCall + self._period
            time.sleep(self._nextCall - time.time())


manager = NodeManager()
listener = DataListener(manager)
plotter = Plotter(manager)
listener.start()
plt.show()
