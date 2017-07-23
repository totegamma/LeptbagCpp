import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 3.3, 0.1)


omega_one = 0.3
omega_two = 0.1

agent_one = np.random.rand(np.shape(x)[0], 1)
agent_one_single = np.random.rand(np.shape(x)[0], 1)
agent_two = np.random.rand(np.shape(x)[0], 1)
agent_two_single = np.random.rand(np.shape(x)[0], 1)

k = np.random.rand(10, 1)
k = k

for i in range(np.shape(x)[0]-1):
    dif = agent_two[i] - agent_one[i]

    y = [ np.sin(dif), np.cos(dif),
        np.sin(2*dif), np.cos(2*dif),
        np.sin(3*dif), np.cos(3*dif),
        np.sin(4*dif), np.cos(4*dif),
        np.sin(5*dif), np.cos(5*dif) ]

    y = y * k[0]
    y = np.sum(y, axis=0)
    agent_one[i+1] = agent_one[i] + omega_one + y
    agent_one_single[i+1] = agent_one_single[i] + np.sin(omega_one)

    omega_one += omega_one


    dif = agent_one[i] - agent_two[i]

    y = [ np.sin(dif), np.cos(dif),
        np.sin(2*dif), np.cos(2*dif),
        np.sin(3*dif), np.cos(3*dif),
        np.sin(4*dif), np.cos(4*dif),
        np.sin(5*dif), np.cos(5*dif) ]

    y = y * k[0]
    y = np.sum(y, axis=0)
    agent_two[i+1] = agent_two[i] + omega_two + y
    agent_two_single[i+1] = agent_two_single[i] + np.sin(omega_two)

    omega_two += omega_two

#
'''
agent_one = np.sin(agent_one)
agent_two = np.sin(agent_two)
agent_one_single = np.sin(agent_one_single)
agent_two_single = np.sin(agent_two_single)
'''


plt.plot(x, agent_one)
#plt.plot(x, agent_one_single)
plt.plot(x, agent_two)
#plt.plot(x, agent_two_single)
plt.show()
