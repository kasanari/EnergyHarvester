from keras.models import Sequential, InputLayer
from keras.layers import Dense
from simulator import Simulation
from node_manager import Node, Action
import matplotlib.pyplot as plt
import numpy as np


actions = [Action.GATHER, Action.CHARGE]

nodes = []
nodes.append(Node(1, 5))
nodes.append(Node(2, 5))
nodes.append(Node(3, 5))

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
    s = sim.get_state()

    while not done:

        if np.random.random() < eps:
            a = np.random.randint(0, len(actions))      # Explore by picking a random action
        else:
            a = np.argmax(model.predict(s))             # Use network to predict which action to take

        action = actions[a]
        #print(action)
        new_s, r, done = sim.step(action)         # Use selected action to update the environment
        target = r + y * np.max(model.predict(new_s))   # Calculate Q-value based on new state
        target_vec = model.predict(s)[0]                # Get both Q-values for this state
        target_vec[a] = target                          # Update Q-value for the action we took

        model.fit(s, target_vec.reshape(-1, 2), epochs=1, verbose=0) # Fit neural network to predict the Q-values


        #print(s)
        s = new_s # Update state
        r_sum += r # Add reward to sum of rewards

    r_avg_list.append(r_sum)

plt.figure()
plt.plot(r_avg_list)
plt.show()