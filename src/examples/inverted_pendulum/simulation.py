"""
This programs runs a simulation of a cart-pole system.
The control algorithm used to balance the pendulum is PID
To start this simulation run:
    cd ~/OpenSCADA
    bazel run :simulation -- --plc_spec_file=<system spec file>
        --plc_spec_dir=<directory containing plc system spec file> \
        --virtual
"""

import argparse
import os
import signal
import sys

from contrib.emulation_driver import EmulationDriver
from examples.inverted_pendulum.pendulum_sim import PendulumSimulator

stop = False


def handler(signum, frame):
    global stop
    print('Pressed Ctrl-C! Scheduled clean exit ...')
    stop = True


def start_grpc_server(path_to_plc_specifications_dir, log_file_fd):
    newpid = os.fork()

    if newpid == 0:
        os.dup2(log_file_fd, sys.stdout.fileno())
        os.dup2(log_file_fd, sys.stderr.fileno())

        # We change process group here so that any signal sent to the 
        # main process doesn't automatically affect all forked children
        # This is necessary for a clean exit if interrupted
        os.setpgrp()
        args = ['pc_grpc_server', path_to_plc_specifications_dir]
        os.execvp(args[0], args)
    else:
        print('Started PC GRPC Server with pid ', newpid)
        return newpid


def start_plc(path_to_plc_specification_file, virtual, rel_cpu_speed,
              n_insns_per_round, log_file_fd):
    newpid = os.fork()
    if newpid == 0:
        os.dup2(log_file_fd, sys.stdout.fileno())
        os.dup2(log_file_fd, sys.stderr.fileno())

        args = ['plc_runner', '-f', path_to_plc_specification_file]
        if virtual:
            # We change process group here so that any signal sent to the 
            # main process doesn't automatically affect all forked children
            # This is necessary for a clean exit if interrupted
            os.setpgrp()
            args.extend([
                '-e', '1',
                '-n', str(n_insns_per_round),
                '-s', str(rel_cpu_speed),
            ])
        os.execvp(args[0], args)
    else:
        print('Started PLC Runner with pid ', newpid)
        return newpid


def start_comm_module(path_to_plc_specification_file, ip_address_to_listen,
                      listen_port, resource_to_attach, virtual, rel_cpu_speed,
                      n_insns_per_round, log_file_fd):
    newpid = os.fork()

    if newpid == 0:
        args = [
            'modbus_comm_module',
            '-f', path_to_plc_specification_file,
            '-i', ip_address_to_listen,
            '-p', listen_port,
            '-r', resource_to_attach,
        ]
        os.dup2(log_file_fd, sys.stdout.fileno())
        os.dup2(log_file_fd, sys.stderr.fileno())
        if virtual:
            # We change process group here so that any signal sent to the 
            # main process doesn't automatically affect all forked children
            # This is necessary for a clean exit if interrupted
            os.setpgrp()
            args = [
                'tracer',
                '-c', ' '.join(args),
                '-r', str(rel_cpu_speed),
                '-n', str(n_insns_per_round),
            ]
        os.execvp(args[0], args)
    else:
        print('Started Modbus comm module with pid ', newpid)
        return newpid


def start_example_hmi(virtual, rel_cpu_speed,
                      n_insns_per_round, log_file_fd):
    newpid = os.fork()
    if newpid == 0:
        args = ['example_hmi']
        os.dup2(log_file_fd, sys.stdout.fileno())
        os.dup2(log_file_fd, sys.stderr.fileno())
        if virtual:
            args = [
                'tracer',
                '-c', 'example_hmi',
                '-r', str(rel_cpu_speed),
                '-n', str(n_insns_per_round),
            ]
            # We change process group here so that any signal sent to the 
            # main process doesn't automatically affect all forked children
            # This is necessary for a clean exit if interrupted
            os.setpgrp()
        os.execvp(args[0], args)
    else:
        print('Started example hmi with pid ', newpid)
        return newpid


def main(num_dilated_nodes=1,
         run_time=5,
         rel_cpu_speed=1.0,
         num_insns_per_round=1000000):
    global stop

    parser = argparse.ArgumentParser()

    parser.add_argument('--plc_spec_file', dest='plc_spec_file',
                        help='path to plc spec prototxt file')

    parser.add_argument('--virtual', dest='virtual', action='store_true', default='False',
                        help='with Kronos', )

    parser.add_argument('--plc_spec_dir', dest='plc_spec_dir',
                        help='path to directory containing spec protxt files of all plcs')

    parser.add_argument('--comm_module_bind_ip', dest='comm_module_bind_ip',
                        help='ip_address to which comm module would bind',
                        default='0.0.0.0')

    parser.add_argument('--comm_module_listen_port', dest='comm_module_listen_port',
                        help='listen port of comm module', default='1502')

    parser.add_argument('--comm_module_attached_resource',
                        dest='comm_module_attached_resource',
                        help='comm module attaches to this resource of the plc',
                        default='CPU_001')

    if os.path.exists('/tmp/pc_grpc_server_log.txt'):
        os.remove('/tmp/pc_grpc_server_log.txt')

    if os.path.exists('/tmp/plc_log.txt'):
        os.remove('/tmp/plc_log.txt')

    if os.path.exists('/tmp/comm_module_log.txt'):
        os.remove('/tmp/comm_module_log.txt')

    if os.path.exists('/tmp/example_hmi.txt'):
        os.remove('/tmp/example_hmi.txt')

    fd1 = os.open('/tmp/pc_grpc_server_log.txt', os.O_RDWR | os.O_CREAT)
    fd2 = os.open('/tmp/plc_log.txt', os.O_RDWR | os.O_CREAT)
    fd3 = os.open('/tmp/comm_module_log.txt', os.O_RDWR | os.O_CREAT)
    fd4 = os.open('/tmp/example_hmi.txt', os.O_RDWR | os.O_CREAT)

    args = parser.parse_args()

    pendulum_sim = PendulumSimulator()
    emulation = EmulationDriver(number_dilated_nodes=num_dilated_nodes,
                                is_virtual=args.virtual,
                                n_insns_per_round=num_insns_per_round,
                                rel_cpu_speed=rel_cpu_speed,
                                physical_system_sim_driver=pendulum_sim)

    # Start pc_grpc_server, all PLCs and all communication modules here 
    print('Starting PC GRPC Server ...')
    grpc_server_pid = start_grpc_server(args.plc_spec_dir, fd1)

    print('Starting PLC ...')
    plc_pid = start_plc(args.plc_spec_file,
                        args.virtual,
                        rel_cpu_speed,
                        num_insns_per_round,
                        fd2)

    # Wait until all processes have started and registered themselves
    emulation.wait_for_initialization()
    signal.signal(signal.SIGINT, handler)

    total_time_elapsed = 0.0
    while total_time_elapsed <= run_time:

        emulation.run_for(0.01)
        total_time_elapsed += 0.01
        if args.virtual:
            print('Time Elapsed: ', total_time_elapsed)
        if stop:
            break

    print('Stopping Emulation ...')
    sys.stdout.flush()
    emulation.stop_exp()

    os.close(fd1)
    os.close(fd2)
    os.close(fd3)
    os.close(fd4)

    pendulum_sim.finish_video()

    print('Interrupting all spawned processes !')
    os.kill(grpc_server_pid, signal.SIGINT)

    if args.virtual:
        os.kill(plc_pid, signal.SIGINT)

    print('Emulation finished !')


if __name__ == '__main__':
    main()
