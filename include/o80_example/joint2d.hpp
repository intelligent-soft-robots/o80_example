#pragma once

#include "o80/statexd.hpp"

namespace o80_example
{
class Joint2d : public o80::StateXd<Joint2d, double, int>
{
public:
    Joint2d() : o80::StateXd<Joint2d, double, int>(0, 0)
    {
    }
    Joint2d(double v1, int v2) : o80::StateXd<Joint2d, double, int>(v1, v2)
    {
    }
};
}  // namespace o80_example
