import sys
import logging
import time
import o80
import o80_example
import signal_handler
from functools import partial
from fyplot import function_plot

SEGMENT_ID = "o80_example_bursting"
DRIVER_MIN = 0.0
DRIVER_MAX = 100.0
WINDOW = (1200, 800)


def _get_plot(frontend):

    plt = function_plot.Plot(SEGMENT_ID, 0.1, WINDOW)

    class Iter:
        iteration = 0

    def get_joint(dof):
        latest = frontend.latest().get_iteration()
        if latest > Iter.iteration:
            obs = frontend.read(Iter.iteration)
            if dof == 1:
                Iter.iteration += 1
            return obs.get_desired_states().get(dof).get()
        return None

    desired_states_plot = (
        (partial(get_joint, 0), (0, 255, 0)),
        (partial(get_joint, 1), (255, 0, 0)),
    )

    plt.add_subplot((DRIVER_MIN, DRIVER_MAX), 1000, desired_states_plot)

    return plt


def run():

    log_handler = logging.StreamHandler(sys.stdout)
    logging.basicConfig(
        format=str(
            "[o80 Example | backend on segment_id: {} |"
            + " %(levelname)s %(asctime)s] %(message)s]"
        ).format(SEGMENT_ID),
        level=logging.DEBUG,
        handlers=[log_handler],
    )

    logging.info("clearning shared memory on {}".format(SEGMENT_ID))
    o80.clear_shared_memory(SEGMENT_ID)

    logging.info("starting o80 standalone (bursting mode)")
    o80_example.start_standalone(
        SEGMENT_ID,
        1.0,  # frequency: ignored in bursting mode
        True,  # bursting mode
        DRIVER_MIN,
        DRIVER_MAX,
    )

    logging.info("instantiating frontend for plotting")
    frontend = o80_example.FrontEnd(SEGMENT_ID)

    logging.info("starting plotting")
    plot = _get_plot(frontend)
    plot.start()

    signal_handler.init()  # for detecting ctrl+c
    iteration = 0
    try:
        while not signal_handler.has_received_sigint():
            # reading the latest iteration
            latest = frontend.latest().get_iteration()
            # new iteration: we should print what is new
            if latest > iteration:
                # reading latest observation
                obs = frontend.read(iteration)
                iteration += 1
                # print current desired joint state
                ds = obs.get_desired_states()
                report = "iteration: {} | \t\t{}\t\t|\t\t{}".format(
                    iteration, ds.get(0).get(), ds.get(1).get()
                )
                print(report)
            # running as fast as possiblea
            time.sleep(0.0001)
    except (KeyboardInterrupt, SystemExit):
        logging.info("exiting ...")
    except Exception as e:
        logging.error(str(e))

    logging.info("stopping plot")
    plot.stop()
    logging.info("stopping o80 backend")
    o80_example.stop_standalone(SEGMENT_ID)
    logging.info("exiting")


if __name__ == "__main__":
    run()
