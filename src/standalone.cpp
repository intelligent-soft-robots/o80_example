#include "o80_example/standalone.hpp"

namespace o80_example
{
Standalone::Standalone(std::shared_ptr<Driver> driver_ptr,
                       double frequency,
                       std::string segment_id)
    : o80::Standalone<o80_EXAMPLE_QUEUE_SIZE,
                      2,
                      Driver,
                      Joint,
                      o80::VoidExtendedState>(driver_ptr, frequency, segment_id)
{
}

DriverIn Standalone::convert(const o80::States<2, Joint> &joints)
{
    DriverIn action;
    for (size_t dof = 0; dof < 2; dof++)
    {
        const Joint &joint = joints.get(dof);
        action.values[dof] = joint.value;
    }
    return action;
}

// robot_interface observation and o80 observation
// are the same thing
o80::States<2, Joint> Standalone::convert(const DriverOut &observation)
{
    o80::States<2, Joint> states;
    states.set(0, Joint(observation.values[0]));
    states.set(1, Joint(observation.values[1]));
    return states;
}

void Standalone::enrich_extended_state(o80::VoidExtendedState &extended_state,
                                       const DriverOut &observation)
{
}

std::string get_segment_id(int id)
{
    std::string segment_id(o80_EXAMPLE_SEGMENT);
    segment_id = segment_id + std::string("_") + std::to_string(id);
    return segment_id;
}
}  // namespace o80_example
