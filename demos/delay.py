import time
import o80
import o80_example

SEGMENT_ID = "o80_example"

# creating an o80 frontend, to send
# commands to the backend
frontend = o80_example.FrontEnd(SEGMENT_ID)

# a trajectory point is a value associated
# to a duration over which this value should be
# reached
class TrajectoryPoint:
    def __init__(self,duration,value):
        self.duration = duration
        self.value = value

# a full trajectory of points
trajectory = [ TrajectoryPoint(250,60),
               TrajectoryPoint(250,50),
               TrajectoryPoint(250,70),
               TrajectoryPoint(250,50),
               TrajectoryPoint(250,80),
               TrajectoryPoint(250,50),
               TrajectoryPoint(250,90),
               TrajectoryPoint(250,50),
               TrajectoryPoint(250,100),
               TrajectoryPoint(250,0) ]

# o80 frontend will send (desired) states
# commands to the backend
state = o80_example.State()

# loading locally the full trajectory 
# for execution on the first (index 0) joint
starting_iteration = frontend.latest().get_iteration()
for tp in trajectory:
    state.set(tp.value)
    frontend.add_command(0, # joint index
                         state,
                         o80.Duration_us.milliseconds(tp.duration),
                         o80.Mode.QUEUE)
# sending the queue of commands to the backend, and
# waiting for end of execution
frontend.pulse_and_wait()

# replaying the trajectory of the first joint
# on the second joint (index 1) with 2 seconds shift.
# Reading all observations from starting_iteration
observations = frontend.get_observations_since(starting_iteration)
# waiting 2 seconds
time.sleep(2)
# loading the corresponding trajectory locally
# for execution on the second joint
for observation in observations:
    joint1_state = observation.get_desired_states().get(0)
    frontend.add_command(1,joint1_state,
                         o80.Mode.QUEUE)
# sending the queue of commands to the backend, and
# waiting for end of execution
frontend.pulse_and_wait()



