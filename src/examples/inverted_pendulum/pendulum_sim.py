import collections
import math

import cv2
import grpc
import numpy as np
import pc_emulator.proto.mem_access_pb2 as mem_access_proto
import pc_emulator.proto.mem_access_pb2_grpc as mem_access_grpc

from contrib.physical_system_sim import PhysicalSystemSim
from examples.models import Cart, Pendulum


class PendulumSimulator(PhysicalSystemSim):

    def __init__(self, record=False):
        self.mass_of_ball = 1.0
        self.mass_of_cart = 5.0
        self.g = 9.81
        self.errors, self.force, self.theta, self.times, self.x = [], [], [], [], []
        self.world_size = 1000

        # Initializing cart and pendulum objects
        self.cart = Cart(int(0.5 * self.world_size), self.mass_of_cart, self.world_size)
        self.pendulum = Pendulum(1, 1, self.mass_of_ball)

        # Initializing other variables needed for the simulation
        self.theta_dot = 0
        self.theta_tm1 = self.theta_tm2 = self.pendulum.theta
        self.x_tm1 = self.x_tm2 = self.cart.x
        self.previous_time_delta = 0
        self.out = None

        if record:
            self.out = cv2.VideoWriter('/tmp/simulation.mp4',
                                       cv2.VideoWriter_fourcc(*'MP4V'),
                                       15,
                                       (self.world_size, self.world_size))
        self.im_array = []
        self.im_count = 1

    def display_stuff(self, delay=5, display_name='Pendulum and Cart'):
        # This function displays the pendulum and cart.
        display_length = self.pendulum.length * 100
        A = np.zeros((self.world_size, self.world_size, 3), np.uint8)
        cv2.line(
            A,
            (0, int(0.6 * self.world_size)),
            (self.world_size, int(0.6 * self.world_size)),
            (255, 255, 255),
            2,
        )
        cv2.rectangle(
            A,
            (int(self.cart.x) + 25, self.cart.y + 15),
            (int(self.cart.x) - 25, self.cart.y - 15),
            self.cart.color,
            -1,
        )
        pendulum_x_endpoint = int(self.cart.x - display_length * math.sin(self.pendulum.theta))
        pendulum_y_endpoint = int(self.cart.y - display_length * math.cos(self.pendulum.theta))
        cv2.line(
            A,
            (int(self.cart.x), self.cart.y),
            (pendulum_x_endpoint, pendulum_y_endpoint),
            self.pendulum.color,
            4,
        )
        cv2.circle(
            A,
            (pendulum_x_endpoint, pendulum_y_endpoint),
            6,
            (255, 255, 255),
            -1,
        )

        name = display_name
        if isinstance(display_name, collections.Callable):
            name = display_name()
        cv2.imshow(name, A)
        cv2.waitKey(delay)

    def apply_control_input(self, F, time_delta):
        # Finding x and theta on considering the control inputs and the dynamics of the system

        theta_double_dot = ((
                                    ((self.cart.mass + self.pendulum.ball_mass) *
                                     self.g *
                                     math.sin(self.pendulum.theta)) +
                                    F *
                                    math.cos(self.pendulum.theta) -
                                    (self.pendulum.ball_mass *
                                     self.theta_dot ** 2.0 *
                                     self.pendulum.length *
                                     math.sin(self.pendulum.theta) *
                                     math.cos(self.pendulum.theta))) /
                            (self.pendulum.length * (self.cart.mass +
                                                     self.pendulum.ball_mass *
                                                     math.sin(self.pendulum.theta) ** 2.0)))

        x_double_dot = ((
                                (self.pendulum.ball_mass *
                                 self.g *
                                 math.sin(self.pendulum.theta) *
                                 math.cos(self.pendulum.theta)) -
                                (self.pendulum.ball_mass *
                                 self.pendulum.length *
                                 math.sin(self.pendulum.theta) *
                                 self.theta_dot ** 2) +
                                F) /
                        (self.cart.mass +
                         (self.pendulum.ball_mass * math.sin(self.pendulum.theta) ** 2)))

        self.cart.x += (
                ((time_delta ** 2) * x_double_dot) +
                (((self.cart.x - self.x_tm2) * time_delta) / self.previous_time_delta))

        self.pendulum.theta += (
                (time_delta ** 2 * theta_double_dot) +
                (((self.pendulum.theta - self.theta_tm2) * time_delta) /
                 self.previous_time_delta))

    # Implementation of abstract class method
    def progress(self, time_step_size):
        if self.previous_time_delta != 0:
            self.theta_dot = (self.theta_tm1 - self.theta_tm2) / self.previous_time_delta

            F = self.get_actuator_output('CPU_001')
            self.apply_control_input(F, time_step_size)

        # Update the variables and display stuff
        self.set_sensor_input(self.pendulum.theta, 'CPU_001')
        self.display_stuff()
        self.previous_time_delta = time_step_size
        self.theta_tm2 = self.theta_tm1
        self.theta_tm1 = self.pendulum.theta
        self.x_tm2 = self.x_tm1
        self.x_tm1 = self.cart.x

    def set_sensor_input(self, curr_theta, resource_name):
        with grpc.insecure_channel('localhost:50051') as channel:
            stub = mem_access_grpc.AccessServiceStub(channel)
            response = stub.SetSensorInput(
                mem_access_proto.SensorInput(
                    PLC_ID='Pendulum_PLC',
                    ResourceName=resource_name,
                    MemType=0,
                    ByteOffset=1,
                    BitOffset=0,
                    VariableDataTypeName='REAL',
                    ValueToSet=str(curr_theta)))

    def get_actuator_output(self, resource_name):
        with grpc.insecure_channel('localhost:50051') as channel:
            stub = mem_access_grpc.AccessServiceStub(channel)
            response = stub.GetActuatorOutput(
                mem_access_proto.ActuatorOutput(
                    PLC_ID='Pendulum_PLC',
                    ResourceName=resource_name,
                    MemType=1,
                    ByteOffset=1,
                    BitOffset=0,
                    VariableDataTypeName='REAL'))

            if response.status == 'SUCCESS':
                return float(response.value)
            return 0.0

    def finish_video(self):
        for im in self.im_array:
            self.out.write(im)
        self.out.release()
