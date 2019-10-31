class Cart(object):
    __slots__ = ['x', 'y', 'mass', 'color']

    def __init__(self, x, mass, world_size):
        self.x = x
        self.y = int(0.6 * world_size)
        self.mass = mass
        self.color = (0, 255, 0)


class Pendulum(object):
    __slots__ = ['length', 'theta', 'ball_mass', 'color']

    def __init__(self, length, theta, ball_mass):
        self.length = length
        self.theta = theta
        self.ball_mass = ball_mass
        self.color = (0, 0, 255)
