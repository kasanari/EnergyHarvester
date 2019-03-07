from keras.models import Sequential, InputLayer
from keras.layers import Dense
from node_manager import Node, Action
from simulator import Simulation
import numpy as np
from collections import deque

class Memory():
    def __init__(self, max_size):
        self.buffer = deque(maxlen=max_size)

    def add(self, experience):
        self.buffer.append(experience)

    def sample(self):
        buffer_size = len(self.buffer)
        return self.buffer[np.random.randint(0, buffer_size)]

class LearningAgent:

    def __init__(self, number_of_nodes):
        self.actions = [Action.GATHER, Action.CHARGE]

        self.lr = 0.95 # Learning rate
        self.expr = 0.1  # Exploration rate

        self.memory = Memory(1000000)
        self.model = Sequential()
        self.model.add(InputLayer(batch_input_shape=(1, number_of_nodes)))
        self.model.add(Dense(5, activation='sigmoid'))
        self.model.add(Dense(len(self.actions), activation='linear'))
        self.model.compile(loss='mse', optimizer='adam', metrics=['mae'])

        self.state = None

    def pretrain(self, nodes, pretrain_length):
        sim = Simulation(nodes)
        state = sim.get_state()
        for i in range(pretrain_length):
            a = np.random.randint(0, len(self.actions)) # Random action
            new_state, reward, done = sim.step(self.actions[a])

            if done:
                new_state = np.zeros(state.shape)
                self.memory.add((state, a, reward, new_state, done)) # Add experience to memory
                sim = Simulation(nodes) # Start a new episode
                state = sim.get_state() # First we need a state

            else:
                self.memory.add((state, a, reward, new_state, done)) # Add experience to memory
                state = new_state # Our state is now the next_state

    def make_action(self, state):
        self.prev_state = state #Store old state for learning

        if np.random.random() < self.expr:
            a = np.random.randint(0, len(self.actions))  # Explore by picking a random action
        else:
            a = np.argmax(self.model.predict(state))  # Use network to predict which action to take
        self.prev_a = a
        print(self.actions[a])
        return self.actions[a]

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

    def learn(self, new_state):
        reward = self.calculate_reward(new_state, self.prev_a)
        self.memory.add((self.prev_state, self.prev_a, reward, new_state)) # Add experience to memory
        state, reward, action, new_state, _ = self.memory.sample()

        Q_target = reward + self.lr * np.max(self.model.predict(new_state))   # Calculate Q-value based on new state
        Q_values = self.model.predict(state)[0]                         # Get both Q-values for this state
        Q_values[action] = Q_target                                     # Update Q-value for the action we took

        self.model.fit(state, Q_values.reshape(-1, 2), epochs=1, verbose=0) # Fit neural network to predict the Q-values
