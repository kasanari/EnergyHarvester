from node_manager import NodeManager, Node
from serial_interface import Action
import time


class Simulation:
    def __init__(self, nodes):
        self.nodes = nodes
        self.node_manager = NodeManager()
        for node in self.nodes:
            self.node_manager.add_node(node)
            self.node_manager.update_node(node)
        self.node_manager.commit(time.time())

    def step(self, actions):
        for index, node in enumerate(self.nodes):
            new_energy_value = node.update(actions[index])
            if (new_energy_value is not None):
                self.node_manager.update_node(node)
        self.node_manager.commit(time.time())

        new_state = self.get_state()
        reward = self.calculate_reward(new_state)
        done = self.isDone()
        return new_state, reward, done

    def calculate_reward(self, energy_values):
        nulls = energy_values.isnull().sum(axis=1).iloc[0]
        reward = energy_values.mean(axis=1).iloc[0] - 10*nulls
        return reward

    def get_state(self):
        return self.node_manager.get_most_recent_state().fillna(value=0)

    def isDone(self):
        return self.get_state().isnull().sum(axis=1).iloc[0] == len(self.get_state().columns)

# nodes = []
# nodes.append(Node(1, 5))
# nodes.append(Node(2, 5))
# nodes.append(Node(3, 5))
# sim = Simulation(nodes)
# actions = [Action.CHARGE, Action.GATHER, Action.GATHER]
# new_state, reward, done = sim.step(actions)