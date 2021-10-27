import time
import o80
import o80_example

SEGMENT_ID = "o80_example"

# creating an o80 frontend, to send
# commands to the backend
frontend = o80_example.FrontEnd(SEGMENT_ID)


# a trajectory point is two values (one per joint)
# associated to a corresponding speed
class TrajectoryPoint:
    def __init__(self, speed, value1, value2):
        self.speed = speed
        self.value1 = value1
        self.value2 = value2


# a full trajectory
trajectory = [
    TrajectoryPoint(40, 50, 50),
    TrajectoryPoint(40, 100, 0),
    TrajectoryPoint(300, 50, 50),
    TrajectoryPoint(40, 80, 20),
    TrajectoryPoint(80, 20, 80),
    TrajectoryPoint(120, 0, 0),
]


# loading trajectory commands to the local queue
state = o80_example.State()
for tp in trajectory:
    state.set(tp.value1)
    frontend.add_command(0, state, o80.Speed.per_second(tp.speed), o80.Mode.QUEUE)
    state.set(tp.value2)
    frontend.add_command(1, state, o80.Speed.per_second(tp.speed), o80.Mode.QUEUE)

# sending the commands, and waiting for their execution
frontend.pulse_and_wait()
