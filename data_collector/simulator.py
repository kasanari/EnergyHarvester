from node_manager import NodeManager, Node
import time


class Simulation:
    def __init__(self):
        self.nodes = []
        self.node_manager = NodeManager()
        self.nodes.append(Node(1, 5))
        self.nodes.append(Node(2, 5))
        self.nodes.append(Node(3, 5))

    def step(self, actions):
        for index, node in enumerate(self.nodes):
            new_energy_value = node.update(actions[index])
            if (new_energy_value is not None):
                self.node_manager.update_node(self.nodes)
        self.node_manager.commit(time.time())

        new_state = self.node_manager.get_most_recent_state()
        reward = self.calculate_reward(new_state)
        done = False
        return new_state, reward, done

    def calculate_reward(self, energy_values):
        nulls = energy_values.isnull().sum(axis=1)
        return energy_values.mean() - nulls


sim = Simulation()
actions = []
new_state, reward, done = sim.step()