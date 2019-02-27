import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

"""Node states"""
GATHERING = "gathering"
SLEEPING = "sleeping"
CHARGING = "charging"
IDLE = "idle"

class Node:
    """Represents a node in the network"""
    def __init__(self, node_id, energy_level=0, state=SLEEPING, threshold_upper=2.5, threshold_lower=2, charge_rate=0.1, discharge_rate=0.2):
        self.node_id = node_id  # The id of the node
        self.energy_level = energy_level
        self.state = state
        self.threshold_upper = threshold_upper
        self.threshold_lower = threshold_lower
        self.charge_rate = charge_rate
        self.discharge_rate = discharge_rate

    def update(self, new_state):
        """For simulation purposes. simulate sending an order to the node and getting an energy value back."""
        return_value = None
        if ((self.state == IDLE) or (self.state == GATHERING)):
            self.state = new_state

        if self.isCharging() or self.isSleeping():
            self.energy_level += self.charge_rate
        elif self.isGathering():
            self.energy_level -= self.discharge_rate
            return_value = self.energy_level

        if self.energy_level < self.threshold_lower:
            self.state = SLEEPING
        elif self.energy_level > self.threshold_upper:
            self.state = IDLE

        return return_value

    def __repr__(self):
        return f"[{self.node_id}, {self.energy_level}]"

    def __str__(self):
        return f"Node with id {self.node_id} and {self.energy_level}V"

    def isCharging(self):
        return self.state == CHARGING
    def isIdle(self):
        return self.state == IDLE
    def isGathering(self):
        return self.state == GATHERING
    def isSleeping(self):
        return self.state == SLEEPING

class NodeManager:
    """Manages a table of nodes contained in the network"""
    max_size = 10

    
    def __init__(self):
        self.number_of_nodes = 0
        self.history = pd.DataFrame(columns=['timestamp'])
        self.history.set_index('timestamp', inplace=True)
        self.data_cache = {}
    
    def remove_node(self, node_id):
        self.nodes = self.history.drop(index=node_id)

    def add_node(self, node):
        length = len(self.history.index)
        self.history[node.node_id] = np.repeat(np.nan, length)
        self.number_of_nodes += 1

    def update_node(self, node):
        self.data_cache[node.node_id] = node.energy_level

    def get_energy_levels(self):
        columns = self.history.columns[:-1]
        return self.history[columns]

    def get_most_recent_state(self):
        return self.history.tail(1)[self.history.columns[:-1]]

    def node_is_in_system(self, node_id):
        try:
            value = self.history[node_id]
        except KeyError:
            return False

        return value is not None

    def commit(self, timestamp):

        real_t = pd.to_datetime(timestamp, unit='s')
        timestamp_dict = {'timestamp':real_t}
        self.history = self.history.append({**timestamp_dict,**self.data_cache}, ignore_index=True)
        self.data_cache = {}

    def reset(self):
        self.data_cache = {}

