from keras.models import Sequential, InputLayer
from keras.layers import Dense
from node_manager import Node, Action
import numpy as np

class LearningAgent:

    def __init__(self, number_of_nodes):
        self.actions = [Action.GATHER, Action.CHARGE]

        self.y = 0.95 # Learning rate
        self.eps = 0.5  # Exploration rate

        self.model = Sequential()
        self.model.add(InputLayer(batch_input_shape=(1, number_of_nodes)))
        self.model.add(Dense(5, activation='sigmoid'))
        self.model.add(Dense(len(self.actions), activation='linear'))
        self.model.compile(loss='mse', optimizer='adam', metrics=['mae'])

        self.state = None

    def make_action(self, state):
        self.prev_state = state #Store old state for learning

        if np.random.random() < self.eps:
            a = np.random.randint(0, len(self.actions))  # Explore by picking a random action
        else:
            a = np.argmax(self.model.predict(state))  # Use network to predict which action to take
        self.prev_a = a
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
        r = self.calculate_reward(new_state, self.prev_a)
        target = r + self.y * np.max(self.model.predict(new_state))    # Calculate Q-value based on new state
        target_vec = self.model.predict(self.prev_state)[0]            # Get both Q-values for this state
        target_vec[self.prev_a] = target                               # Update Q-value for the action we took

        self.model.fit(self.prev_state, target_vec.reshape(-1, 2), epochs=1, verbose=0)  # Fit neural network to predict the Q-values