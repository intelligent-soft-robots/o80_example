import time
import o80
import o80_example

SEGMENT_ID = "o80_example"

frontend = o80_example.FrontEnd(SEGMENT_ID)

state = o80_example.State()
v = 1

while True:

    for _ in range(10):
        state.set(v)
        frontend.add_command(0,state,o80.Mode.QUEUE)
        frontend.add_command(0,state,o80.Mode.QUEUE)
        v+=1
        if v>100:
            v=1
        
    frontend.pulse_and_wait()


