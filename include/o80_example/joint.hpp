#pragma once

#include "o80/state.hpp"

namespace o80_example
{
  class Joint : public o80::State<double,Joint>
  {
  public:
    Joint(): o80::State<double,Joint>(0){}
    Joint(double value) : o80::State<double,Joint>(value){}
  };
}
