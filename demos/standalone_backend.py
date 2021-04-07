import sys
import time
import logging
import signal_handler
import o80
import o80_example
from fyplot import function_plot

SEGMENT_ID = "o80_example"
FREQUENCY = 5000
DRIVER_MIN = 0.
DRIVER_MAX = 100.
WINDOW = (1200,800)


def _get_plot(frontend):

    plt = function_plot.Plot(SEGMENT_ID,50,WINDOW)

    def get_joint1():
        return frontend.latest().get_desired_states().get(0).get()

    def get_joint2():
        return frontend.latest().get_desired_states().get(1).get()

    def get_frequency():
        return frontend.latest().get_frequency()

    desired_states_plot = ( (get_joint1,(0,255,0)),
                            (get_joint2,(255,0,0)) )
    frequency_plot = ((get_frequency,(255,0,0)),)
    plt.add_subplot((DRIVER_MIN,DRIVER_MAX),1000,desired_states_plot)
    plt.add_subplot((0,FREQUENCY+100),1000,frequency_plot)

    return plt

    

def _run():

    log_handler = logging.StreamHandler(sys.stdout)
    logging.basicConfig(
        format=str("[o80 Example | segment_id: {} | bursting_mode: False |"+
                   " %(levelname)s %(asctime)s] %(message)s").format(SEGMENT_ID),
        level=logging.DEBUG,
        handlers=[log_handler]
    )

        
    logging.info("clearning shared memory on {}".format(SEGMENT_ID))
    o80.clear_shared_memory(SEGMENT_ID)

    logging.info("starting o80 standalone with frequency {}".format(FREQUENCY))
    o80_example.start_standalone(SEGMENT_ID,
                                 FREQUENCY,
                                 False, # Bursting mode
                                 DRIVER_MIN,
                                 DRIVER_MAX)

    logging.info("instantiating frontend for plotting")
    frontend = o80_example.FrontEnd(SEGMENT_ID)

    logging.info("starting plotting")
    plot = _get_plot(frontend)
    plot.start()

    signal_handler.init() # for detecting ctrl+c
    try:
        time_c = time.time()-6
        while not signal_handler.has_received_sigint():
            time.sleep(0.01)
            if time.time()-time_c > 5:
                logging.info("running ...")
                time_c = time.time()
    except (KeyboardInterrupt,SystemExit):
        logging.info("exiting ...")
    except Exception as e:
        logging.error(str(e))

    logging.info("stopping plot")
    plot.stop()
    logging.info("stopping o80 backend")
    o80_example.stop_standalone(SEGMENT_ID)
    logging.info("exiting")


if __name__ == "__main__":

    _run()
