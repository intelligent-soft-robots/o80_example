"""
Creates a frontend and print some statistics about the
backend's frequencies.
"""

import numpy
import o80_example

SEGMENT_ID = "o80_example"


def run():

    frontend = o80_example.FrontEnd(SEGMENT_ID)

    # backend frequency and allowed limits
    expected_frequency = frontend.get_frequency()
    five_percent = expected_frequency * 0.05
    ten_percent = expected_frequency * 0.1
    up_tolerance1 = expected_frequency + five_percent
    up_tolerance2 = expected_frequency + ten_percent
    down_tolerance1 = expected_frequency - five_percent
    down_tolerance2 = expected_frequency - ten_percent
    
    # getting all observations
    observations = frontend.get_latest_observations(100000)
    nb_observations = len(observations)

    # computing the frequency of each step
    time_stamps = [o.get_time_stamp() for o in observations]
    periods = [(ts2 - ts1) * 1e-9 for ts1, ts2 in zip(time_stamps, time_stamps[1:])]
    frequencies = [1.0 / p for p in periods]

    # some stats
    max_frequency = max(frequencies)
    min_frequency = min(frequencies)
    up_spikes1 = [f for f in frequencies if f > up_tolerance1]
    nb_up_spikes1 = len(up_spikes1)
    up_spikes2 = [f for f in frequencies if f > up_tolerance2]
    nb_up_spikes2 = len(up_spikes2)
    down_spikes1 = [f for f in frequencies if f < down_tolerance1]
    nb_down_spikes1 = len(down_spikes1)
    down_spikes2 = [f for f in frequencies if f < down_tolerance2]
    nb_down_spikes2 = len(down_spikes2)
    decent_frequencies = [
        f for f in frequencies if f <= up_tolerance1 and f >= down_tolerance1
    ]
    average = sum(decent_frequencies) / len(decent_frequencies)
    std = numpy.std(numpy.array(decent_frequencies))

    # printing results
    print()
    print("Frequency monitoring, over {} iterations".format(nb_observations))
    print("\texpected frequency: {}".format(expected_frequency))
    print(
        "\tspikes over {}: {} ({}%)".format(
            up_tolerance1, nb_up_spikes1, float(nb_up_spikes1) / nb_observations
        )
    )
    print(
        "\tspikes over {}: {} ({}%)".format(
            up_tolerance2, nb_up_spikes2, float(nb_up_spikes2) / nb_observations
        )
    )
    print(
        "\tspikes below {}: {} ({}%)".format(
            down_tolerance1, nb_down_spikes1, float(nb_down_spikes1) / nb_observations
        )
    )
    print(
        "\tspikes below {}: {} ({}%)".format(
            down_tolerance2, nb_down_spikes2, float(nb_down_spikes2) / nb_observations
        )
    )
    print("\tmax frequency observed: {}".format(max_frequency))
    print("\tmin frequency observed: {}".format(min_frequency))
    print("\taverage frequency (spikes excluded): {}".format(average))
    print("\tstandard deviation (spikes excluded): {}".format(std))
    print()


if __name__ == "__main__":
    run()
