from functools import partial
import time
import fyplot
import o80
import o80_example

SEGMENT_ID = "o80_example_bursting"
FREQUENCY = 1000
BURSTING_MODE = True # !!!
DRIVER_MIN = 0.
DRIVER_MAX = 100.
WINDOW = (1200,800)


def _get_plot(frontend):

    plt = fyplot.Plot(SEGMENT_ID,0.1,WINDOW)

    class Iter:
        iteration = 0
    
    def get_joint(dof):
        latest = frontend.latest().get_iteration()
        if latest>Iter.iteration:
            obs = frontend.read(Iter.iteration)
            if dof==1:
                Iter.iteration+=1
            return obs.get_desired_states().get(dof).get()
        return None
    
    desired_states_plot = ( (partial(get_joint,0),(0,255,0)),
                            (partial(get_joint,1),(255,0,0)) )
    
    plt.add_subplot((DRIVER_MIN,DRIVER_MAX),1000,desired_states_plot)
    
    return plt


def run():

    o80.clear_shared_memory(SEGMENT_ID)

    o80_example.start_standalone(SEGMENT_ID,
                                 FREQUENCY,
                                 BURSTING_MODE,
                                 DRIVER_MIN,
                                 DRIVER_MAX)
    
    frontend = o80_example.FrontEnd(SEGMENT_ID)

    plot = _get_plot(frontend)
    plot.start()

    iteration = 0
    try :
        while True:
            latest = frontend.latest().get_iteration()
            if latest > iteration:
                obs = frontend.read(iteration)
                iteration+=1
                ds = obs.get_desired_states()
                print("\t|\t",ds.get(0).get(),"\t|\t",ds.get(1).get(),"\t|\t")
            time.sleep(0.0001)
    except KeyboardInterrupt:
        pass

    plot.stop()
    o80_example.stop_standalone(SEGMENT_ID)

if __name__ == "__main__":
    run()
