from keras.models import Sequential, InputLayer
from keras.layers import Dense
from simulator import Simulation
from node_manager import Node, Action
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
from collections import deque
import random

batch_size = 64
### MEMORY HYPERPARAMETERS
memory_size = 1000000          # Number of experiences the Memory can keep
pretrain_length = 64

class Memory():
    def __init__(self, max_size):
        self.buffer = deque(maxlen=max_size)

    def add(self, experience):
        self.buffer.append(experience)

    def sample(self):
        buffer_size = len(self.buffer)
        return self.buffer[np.random.randint(0, buffer_size)]


actions = [Action.GATHER, Action.CHARGE]

nodes = []
nodes.append(Node(1, energy_level=5))
nodes.append(Node(2, energy_level=5))
nodes.append(Node(3, energy_level=5))

memory = Memory(max_size = memory_size)

def pretrain():
    sim = Simulation(nodes)
    state = sim.get_state()
    for i in range(pretrain_length):
        a = np.random.randint(0, len(actions)) # Random action
        new_state, reward, done = sim.step(actions[a])

        if done:
            # We finished the episode
            new_state = np.zeros(state.shape)
            memory.add((state, a, reward, new_state, done)) # Add experience to memory
            sim = Simulation(nodes) # Start a new episode
            state = sim.get_state() # First we need a state

        else:
            memory.add((state, a, reward, new_state, done)) # Add experience to memory
            state = new_state # Our state is now the next_state

pretrain()
model = Sequential()
model.add(InputLayer(batch_input_shape=(1, len(nodes))))
model.add(Dense(5, activation='sigmoid'))
model.add(Dense(len(actions), activation='linear'))
model.compile(loss='mse', optimizer='adam', metrics=['mae'])
# Q-learning

num_episodes = 1000
y = 0.95
eps = 0.5 # Exploration rate
r_avg_list = []
for i in range(num_episodes):
    if i % 100 == 0:
        print("Episode {} of {}".format(i + 1, num_episodes))
    done = False
    r_sum = 0
    sim = Simulation(nodes)
    state = sim.get_state() # Initial state

    iteration = 0

    while not done:

        eps = 1/math.sqrt(iteration + 1)                # Gradually decrease exploration rate

        if np.random.random() < eps:
            a = np.random.randint(0, len(actions))      # Explore by picking a random action
        else:
            a = np.argmax(model.predict(state))             # Use network to predict which action to take

        action = actions[a]
        #print(action)
        new_state, reward, done = sim.step(action)               # Use selected action to update the environment


        if done:
            new_state = np.zeros(state.shape)
            memory.add((state, a, reward, new_state, done)) # Add experience to memory
        else:
            memory.add((state, a, reward, new_state, done)) # Add experience to memory

        state, reward, action, new_state, done = memory.sample()

        Q_target = reward + y * np.max(model.predict(new_state))   # Calculate Q-value based on new state
        Q_values = model.predict(state)[0]                         # Get both Q-values for this state
        Q_values[a] = Q_target                                     # Update Q-value for the action we took

        model.fit(state, Q_values.reshape(-1, 2), epochs=1, verbose=0) # Fit neural network to predict the Q-values

        #print(state)
        state = new_state # Update state
        r_sum += reward # Add reward to sum of rewards
        iteration += 1

    r_avg_list.append(r_sum)

plt.figure()
plt.plot(r_avg_list)
plt.show()