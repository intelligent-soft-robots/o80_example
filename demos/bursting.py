import time
import o80
import o80_example

SEGMENT_ID = "o80_example_bursting"

frontend = o80_example.FrontEnd(SEGMENT_ID)

class TrajectoryPoint:
    def __init__(self,nb_iterations,value1,value2):
        self.nb_iterations = nb_iterations
        self.value1 = value1
        self.value2 = value2


trajectory = [ TrajectoryPoint(1000,50,100),
               TrajectoryPoint(500,0,0),
               TrajectoryPoint(500,100,50),
               TrajectoryPoint(1000,0,0) ]

state = o80_example.State()
iteration = frontend.latest().get_iteration()
for tp in trajectory:
    iteration += tp.nb_iterations
    state.set(tp.value1)
    frontend.add_command(0,state,
                         o80.Iteration(iteration),
                         o80.Mode.QUEUE)
    state.set(tp.value2)
    frontend.add_command(1,state,
                         o80.Iteration(iteration),
                         o80.Mode.QUEUE)
    frontend.burst(tp.nb_iterations)

    time.sleep(2)

