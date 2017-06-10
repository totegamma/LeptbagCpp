import numpy as np


def phase(theta, omega, coupling):

    upper = np.zeros(8)
    lower = np.zeros(8)
    for the in theta:
        upper += np.sin(theta[0:4] - the)
        lower += np.sin(theta[4:8] - the)

    upper[0:4] = coupling[0] * upper[0:4]
    upper[4:8] = coupling[1] * upper[4:8]
    lower[0:4] = coupling[2] * lower[0:4]
    lower[4:8] = coupling[3] * lower[4:8]



