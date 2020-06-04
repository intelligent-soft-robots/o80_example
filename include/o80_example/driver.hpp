#pragma once

#include <unistd.h>
#include "o80_example/action.hpp"
#include "o80_example/observation.hpp"
#include "robot_interfaces/robot_driver.hpp"

namespace o80_example
{
class Driver : public robot_interfaces::RobotDriver<Action, Observation>
{
public:
    Driver(double min, double max);
    void initialize();
    Action apply_action(const Action &desired_action);
    Observation get_latest_observation();
    void shutdown();
    std::string get_error();

private:
    double values_[2];
    double min_;
    double max_;
};
}
