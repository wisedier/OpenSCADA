from contrib.physical_system_sim import PhysicalSystemSim
from examples.inverted_pendulum.pendulum_sim import PendulumSimulator


class SinglePendulumSimulator(PendulumSimulator):

    def __init__(self, controlling_plc_cpu, record=False):
        super().__init__(record=record)
        self.total_time_elapsed = 0
        self.cpu_id = controlling_plc_cpu
        self.pendulum_angles = []

    def display_stuff(self, delay=1, display_name=None):
        def display_name_getter():
            return self.cpu_id

        if display_name is None:
            display_name = display_name_getter

        super().display_stuff(delay=delay, display_name=display_name)
        self.im_count += 1

    # Implementation of abstract class method
    def progress(self, time_step_size):
        if self.previous_time_delta != 0:
            self.theta_dot = (self.theta_tm1 - self.theta_tm2) / self.previous_time_delta

            F = self.get_actuator_output(self.cpu_id)
            self.apply_control_input(F, time_step_size)

        # Update the variables and display stuff
        self.set_sensor_input(self.pendulum.theta, self.cpu_id)
        if self.total_time_elapsed <= 5.0:
            self.pendulum_angles.append(self.pendulum.theta)
        self.total_time_elapsed += time_step_size

        self.previous_time_delta = time_step_size
        self.theta_tm2 = self.theta_tm1
        self.theta_tm1 = self.pendulum.theta
        self.x_tm2 = self.x_tm1
        self.x_tm1 = self.cart.x

    def display(self):
        self.display_stuff()


class PendulumSystemSimulator(PhysicalSystemSim):
    def __init__(self):
        self.pendulums = []
        self.pendulums.append(SinglePendulumSimulator("CPU_001"))
        self.pendulums.append(SinglePendulumSimulator("CPU_002"))

    # Implementation of abstract class method
    def progress(self, time_step_size):
        self.pendulums[0].progress(time_step_size)
        self.pendulums[1].progress(time_step_size)

    def display(self):
        self.pendulums[0].display()
        self.pendulums[1].display()

    def finish_video(self):
        pass
        # for im in self.pendulums[0].im_array:
        #   self.pendulums[0].out.write(im)
        # self.pendulums[0].out.release()

        # for im in self.pendulums[1].im_array:
        #   self.pendulums[1].out.write(im)
        # self.pendulums[1].out.release()
        with open('/tmp/pendulum-1.txt', 'w') as f:
            for angle in self.pendulums[0].pendulum_angles:
                f.write(str(angle) + '\n')
        with open('/tmp/pendulum-2.txt', 'w') as f:
            for angle in self.pendulums[1].pendulum_angles:
                f.write(str(angle) + '\n')
