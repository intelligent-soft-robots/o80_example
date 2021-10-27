import time
import o80_example
import signal_handler

SEGMENT_ID = "o80_example"


def _run():
    global SEGMENT_ID
    o80_example.Introspector.start(SEGMENT_ID)
    signal_handler.init()  # for detecting ctrl+c
    try:
        while not signal_handler.has_received_sigint():
            time.sleep(0.01)
    except (KeyboardInterrupt, SystemExit):
        o80_example.Introspector.stop()


if __name__ == "__main__":
    _run()
