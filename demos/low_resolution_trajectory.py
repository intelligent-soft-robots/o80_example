import math
import time
import o80
import o80_example

SEGMENT_ID = "o80_example"

# creating an o80 frontend, to send
# commands to the backend
frontend = o80_example.FrontEnd(SEGMENT_ID)

# reading the current backend iteration
initial_iteration = frontend.latest().get_iteration()

# sending a command requesting to reach
# the starting state (50,50) at a given
# future iteration
start_iteration = initial_iteration+1000
state = o80_example.State()
state.set(50)
frontend.add_command(0,state,
                     o80.Iteration(start_iteration),
                     o80.Mode.OVERWRITE)
frontend.add_command(1,state,
                     o80.Iteration(start_iteration),
                     o80.Mode.OVERWRITE)


# creating on the local queue a high resolution
# sinusoid trajectory for joint 0
value=0
increment = 0.001
amplitude = 30
for _ in range(15000):
    value += increment
    v0 = +amplitude*math.sin(value)+50
    state.set(v0)
    frontend.add_command(0,state,
                         o80.Mode.QUEUE)

# creating for joint 1 as similar trajectory,
# but with reduced resolution (by factor 500)
value=0
value_increment = 0.5 # different compared to join0 !
amplitude = 30
target_iteration = start_iteration
iteration_increment = 500
for _ in range(30):
    target_iteration += iteration_increment
    value += value_increment
    v1 = +amplitude*math.sin(value)+50
    state.set(v1)
    frontend.add_command(1,state,
                         o80.Iteration(target_iteration),
                         o80.Mode.QUEUE)

    
# sending a command requesting to reach
# zero over 1 second
state.set(0)
frontend.add_command(0,state,
                     o80.Duration_us.seconds(1),
                     o80.Mode.QUEUE)
frontend.add_command(1,state,
                     o80.Duration_us.seconds(1),
                     o80.Mode.QUEUE)

# sending the commands to the backend for execution
frontend.pulse();


