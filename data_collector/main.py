import serial_interface
import threading
from node_manager import NodeManager, Node
from serial_interface import ADD_NODE_HEADER, DATA_HEADER, REMOVE_NODE_HEADER
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


class Plotter:
    def __init__(self, node_manager):
        self.node_manager = node_manager
        self.bar, = plt.plot(0, 0)
        self.ani = FuncAnimation(plt.gcf(), self.run, interval=1000, repeat=True)

    def run(self, i):
        print("Plotting data")
        energy_levels = self.node_manager.get_energy_levels()
        self.bar.set_data(range(len(energy_levels)), energy_levels)
        self.bar.axes.relim()
        self.bar.axes.autoscale_view()


class DataListener(threading.Thread):
    def __init__(self, node_manager):

        threading.Thread.__init__(self)

        self.node_manager = node_manager

    def run(self):
        while True:
            print("Fetching data from fog node...")
            header, node = serial_interface.receive_message()
            print(f'Header: {header}')
            if header == DATA_HEADER:
                self.node_manager.update_node(node)
            elif header == ADD_NODE_HEADER:
                self.node_manager.add_node(node)
            elif header == REMOVE_NODE_HEADER:
                self.node_manager.remove_node(node.node_id)


manager = NodeManager()
listener = DataListener(manager)
plotter = Plotter(manager)
listener.start()
plt.show()
