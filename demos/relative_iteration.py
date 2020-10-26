import time
import o80
import o80_example


SEGMENT_ID = "o80_example"

# creating an o80 frontend, to send
# commands to the backend
frontend = o80_example.FrontEnd(SEGMENT_ID)


# a trajectory point is two values (one per joint)
# associated to a number of backend iterations over which this value should be
# reached
class TrajectoryPoint:
    def __init__(self,iteration,value1,value2):
        self.iteration = iteration
        self.value1 = value1
        self.value2 = value2

# a full trajectory
trajectory = [ TrajectoryPoint(1000,50,50),
               TrajectoryPoint(1500,60,40),
               TrajectoryPoint(2000,40,60),
               TrajectoryPoint(2500,60,40),
               TrajectoryPoint(3000,40,60),
               TrajectoryPoint(3500,60,40),
               TrajectoryPoint(4000,40,60),
               TrajectoryPoint(4500,60,40),
               TrajectoryPoint(5500,0,0) ]


state = o80_example.State()
# requesting the backend to be in related iteration mode:
# i.e. the iteration will not refer to the absolute iteration
# count of the backend, but to a relative iteration count,
# count which starts when the first command with a "reset"
# iteration is received (see call to iteration.reset() below)
relative = True
# loading the full trajectory in the local queue
# (trajectory to be played twice)
for _ in range(2):
    first_iter = True
    for tp in trajectory:
        state.set(tp.value1)
        iteration = o80.Iteration(tp.iteration,relative)
        if first_iter:
            # this is called only for the first point
            # of the trajectory
            iteration.reset()
            first_iter = False
        frontend.add_command(0,state,
                             iteration,
                             o80.Mode.QUEUE)
        state.set(tp.value2)
        iteration = o80.Iteration(tp.iteration,relative)
        frontend.add_command(1,state,
                             iteration,
                             o80.Mode.QUEUE)
# sending commands to backend
frontend.pulse()


