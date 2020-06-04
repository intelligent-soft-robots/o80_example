#include "o80_example/driver.hpp"

namespace o80_example
{
Driver::Driver(double min, double max) : min_(min), max_(max)
{
}

// at init dof are at min value
void Driver::initialize()
{
    values_[0] = min_;
    values_[1] = min_;
}

// just clip desired values
// between 0 and 1000
Action Driver::apply_action(const Action &desired_action)
{
    Action applied;
    for (unsigned int i = 0; i < 2; i++)
    {
        if (desired_action.values[i] > max_)
        {
            applied.values[i] = max_;
        }
        else if (desired_action.values[i] < min_)
        {
            applied.values[i] = min_;
        }
        else
        {
            applied.values[i] = desired_action.values[i];
        }
        usleep(10);
        values_[i] = applied.values[i];
    }
    return applied;
}

Observation Driver::get_latest_observation()
{
    Observation observation;
    observation.values[0] = values_[0];
    observation.values[1] = values_[1];
    return observation;
}

void Driver::shutdown()
{
}

std::string Driver::get_error()
{
    return "";
}
}
