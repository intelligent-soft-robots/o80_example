#pragma once

#include <unistd.h>
#include "o80/driver.hpp"
#include "o80_example/driver_in.hpp"
#include "o80_example/driver_out.hpp"

namespace o80_example
{
class Driver : public o80::Driver<DriverIn, DriverOut>
{
public:
    Driver(double min, double max);
    void start();
    void stop();
    void set(const DriverIn& in);
    DriverOut get();

private:
    double values_[2];
    double min_;
    double max_;
};
}  // namespace o80_example
