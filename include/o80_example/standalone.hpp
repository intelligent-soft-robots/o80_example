#pragma once

#include "o80/memory_clearing.hpp"
#include "o80/standalone.hpp"
#include "o80_example/driver.hpp"
#include "o80_example/joint.hpp"

#define o80_EXAMPLE_SEGMENT "o80_EXAMPLE"
#define o80_EXAMPLE_QUEUE_SIZE 1000
#define o80_EXAMPLE_NB_DOFS 2

namespace o80_example
{
class Standalone
    : public o80::Standalone<o80_EXAMPLE_QUEUE_SIZE,  // Queue size
                                                      // for command passing
                             o80_EXAMPLE_NB_DOFS,  // 2 dofs robot
                             Driver,
                             Joint,                   // o80 observation
                             o80::VoidExtendedState>  // no info on top of obs
{
public:
    Standalone(std::shared_ptr<Driver> driver_ptr,
               double frequency,
               std::string segment_id);

    DriverIn convert(const o80::States<2, Joint> &joints);

    o80::States<2, Joint> convert(const DriverOut &driver_out);

    // optional user function for adding content to extended_state based
    // on observation. Not needed for this example.
    //void enrich_extended_state(o80::VoidExtendedState &extended_state,
    //                           const DriverOut &driver_out);
};

std::string get_segment_id(int id);
}  // namespace o80_example
