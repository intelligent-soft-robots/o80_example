import time
import o80
import o80_example

SEGMENT_ID = "o80_example"

# creating an o80 frontend, to send
# commands to the backend
frontend = o80_example.FrontEnd(SEGMENT_ID)

# a trajectory point is two values (one per joint)
# associated to a duration over which this value should be
# reached
class TrajectoryPoint:
    def __init__(self, duration, value1, value2):
        self.duration = duration
        self.value1 = value1
        self.value2 = value2


# sending a trajectory to first joint
trajectory = [
    TrajectoryPoint(1000, 100, 0),
    TrajectoryPoint(500, 0, 50),
    TrajectoryPoint(500, 100, 0),
    TrajectoryPoint(1000, 0, 0),
]
state = o80_example.State()
for tp in trajectory:
    state.set(tp.value1)
    frontend.add_command(
        0, state, o80.Duration_us.milliseconds(tp.duration), o80.Mode.QUEUE
    )
frontend.pulse()

# having the second joint mirroring
# the value of the first joint (but with value divided by 2)
time_start = time.time()
while time.time() - time_start < 3:
    # reading current robot observation
    obs = frontend.latest()
    # extracting current desired value for joint 0
    value = obs.get_desired_states().get(0).get() / 2.0
    # setting the value for joint 1
    state.set(value)
    # adding the command to the local queue
    frontend.add_command(1, state, o80.Mode.OVERWRITE)
    # sending the command to the backend,
    # requesting joint1 to be at half the
    # value of joint0
    frontend.pulse()
    # running at approx 1000hz
    time.sleep(0.001)
