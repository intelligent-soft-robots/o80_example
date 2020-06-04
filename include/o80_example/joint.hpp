#pragma once

#include "o80/interpolation.hpp"

namespace o80_example
{
class Joint
{
public:
    Joint(double value_);
    Joint();
    static Joint eval(double value);
    std::string to_string() const;
    double get() const;
    void set(double v);
    bool finished(const o80::TimePoint &start,
                  const o80::TimePoint &now,
                  const Joint &start_state,
                  const Joint &current_state,
                  const Joint &previous_desired_state,
                  const Joint &target_state,
                  const o80::Speed &speed) const;
    Joint intermediate_state(const o80::TimePoint &start,
                             const o80::TimePoint &now,
                             const Joint &start_state,
                             const Joint &current_state,
                             const Joint &previous_desired_state,
                             const Joint &target_state,
                             const o80::Speed &speed) const;
    Joint intermediate_state(const o80::TimePoint &start,
                             const o80::TimePoint &now,
                             const Joint &start_state,
                             const Joint &current_state,
                             const Joint &previously_desired_state,
                             const Joint &target_state,
                             const o80::Duration_us &duration) const;
    Joint intermediate_state(long int iteration_start,
                             long int iteration_now,
                             const Joint &start_state,
                             const Joint &current_state,
                             const Joint &previous_desired_state,
                             const Joint &target_state,
                             const o80::Iteration &iteration) const;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(value);
    }

    double value;
};
}
