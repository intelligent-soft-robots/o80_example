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
    def __init__(self,duration,value1,value2):
        self.duration = duration
        self.value1 = value1
        self.value2 = value2

# a full trajectory
trajectory = [ TrajectoryPoint(2000,50,100),
               TrajectoryPoint(500,0,0),
               TrajectoryPoint(500,100,50),
               TrajectoryPoint(2000,0,0) ]

# loading locally the full trajectory
state = o80_example.State()
for tp in trajectory:
    state.set(tp.value1)
    frontend.add_command(0,state,
                         o80.Duration_us.milliseconds(tp.duration),
                         o80.Mode.QUEUE)
    state.set(tp.value2)
    frontend.add_command(1,state,
                         o80.Duration_us.milliseconds(tp.duration),
                         o80.Mode.QUEUE)

# sending the trajectory to the backend,
# and waiting for end of execution
frontend.pulse_and_wait()


