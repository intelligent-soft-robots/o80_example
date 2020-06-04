#include "o80_example/joint.hpp"

namespace o80_example
{
Joint::Joint(double value_) : value(value_)
{
}

Joint::Joint()
{
}

Joint Joint::eval(double value)
{
    return Joint(value);
}

std::string Joint::to_string() const
{
    return std::to_string(value);
}

double Joint::get() const
{
    return value;
}

void Joint::set(double v)
{
    value = v;
}

bool Joint::finished(const o80::TimePoint &start,
                     const o80::TimePoint &now,
                     const Joint &start_state,
                     const Joint &current_state,
                     const Joint &previous_desired_state,
                     const Joint &target_state,
                     const o80::Speed &speed) const
{
    return o80::finished(start,
                         now,
                         start_state.value,
                         current_state.value,
                         target_state.value,
                         speed);
}

Joint Joint::intermediate_state(const o80::TimePoint &start,
                                const o80::TimePoint &now,
                                const Joint &start_state,
                                const Joint &current_state,
                                const Joint &previous_desired_state,
                                const Joint &target_state,
                                const o80::Speed &speed) const
{
    double desired = o80::intermediate_state(start,
                                             now,
                                             start_state.value,
                                             current_state.value,
                                             target_state.value,
                                             speed);
}

Joint Joint::intermediate_state(const o80::TimePoint &start,
                                const o80::TimePoint &now,
                                const Joint &start_state,
                                const Joint &current_state,
                                const Joint &previously_desired_state,
                                const Joint &target_state,
                                const o80::Duration_us &duration) const
{
    double desired = o80::intermediate_state(start,
                                             now,
                                             start_state.value,
                                             current_state.value,
                                             target_state.value,
                                             duration);
    return Joint(desired);
}

Joint Joint::intermediate_state(long int iteration_start,
                                long int iteration_now,
                                const Joint &start_state,
                                const Joint &current_state,
                                const Joint &previous_desired_state,
                                const Joint &target_state,
                                const o80::Iteration &iteration) const
{
    double desired = o80::intermediate_state(iteration_start,
                                             iteration_now,
                                             start_state.value,
                                             current_state.value,
                                             target_state.value,
                                             iteration);
    return Joint(desired);
}
}
