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
        self.steps = 0

    def step(self, action):
        self.steps += 1
        for index, node in enumerate(self.nodes):
            new_energy_value = node.update(action)
            if (new_energy_value is not None):
                self.node_manager.update_node(node)
        self.node_manager.commit(time.time())

        new_state = self.get_state()
        reward = self.calculate_reward(new_state, action)
        done = self.isDone()
        return new_state, reward, done

    def calculate_reward(self, energy_values, action):
        nulls = energy_values.isnull().sum(axis=1).iloc[0]
        # energy_too_low = self.get_state().mean(axis=1).iloc[0] < 1.8
        for value in energy_values:
            if energy_values[value].iloc[0] < 1.8:
                return -100

        if (nulls > 0):
            reward = -100
        elif (action is Action.CHARGE):
            reward = -1
        elif (action is Action.GATHER):
            reward = 1
        else:
            reward = 0
        return reward

    def get_state(self):
        return self.node_manager.get_most_recent_state().fillna(value=0)

    def isDone(self):
        energy_too_low = (self.get_state().mean(axis=1).iloc[0] < 1.8)
        return energy_too_low or (self.steps > 10000)

# nodes = []
# nodes.append(Node(1, 5))
# nodes.append(Node(2, 5))
# nodes.append(Node(3, 5))
# sim = Simulation(nodes)
# actions = [Action.CHARGE, Action.GATHER, Action.GATHER]
# new_state, reward, done = sim.step(actions)