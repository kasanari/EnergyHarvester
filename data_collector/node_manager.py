import pandas as pd
import matplotlib.pyplot as plt

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

    def update(self): 
        """For simulation purposes. Update the states of the node."""
        if CHARGING:
            self.energy_level += self.charge_rate
        elif GATHERING:
            self.energy_level -= self.discharge_rate

        if self.energy_level < self.threshold_lower:
            self.state = SLEEPING
        elif self.energy_level > self.threshold_upper:
            self.state = IDLE

    def __repr__(self):
        return f"[{self.node_id}, {self.energy_level}]"

    def __str__(self):
        return f"Node with id {self.node_id} and {self.energy_level}V"


class NodeManager:
    """Manages a table of nodes contained in the network"""
    def __init__(self):
        self.number_of_nodes = 0
        self.nodes = pd.DataFrame(columns=['id', 'energy'])
        self.nodes.set_index('id', inplace=True, verify_integrity=True)
    
    def remove_node(self, node_id):
        self.nodes = self.nodes.drop(index=node_id)

    def add_node(self, node):
        self.nodes.loc[node.node_id] = node.energy_level

    def update_node(self, node):
        self.nodes.loc[node.node_id] = node.energy_level

    def get_node(self, node_id):
        return self.nodes.loc[node_id]

    def get_energy_levels(self):
        return self.nodes['energy']

    def node_is_in_system(self, node_id):
        try:
            value = self.nodes.loc[node_id]
        except KeyError:
            return False

        return value is not None

    def plot_nodes(self):
        fig = plt.figure()
        energy_levels = self.nodes['energy']
        plt.bar(range(len(energy_levels)), energy_levels)
        fig.show()
