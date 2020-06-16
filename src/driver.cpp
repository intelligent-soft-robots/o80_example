#include "o80_example/driver.hpp"

namespace o80_example
{
Driver::Driver(double min, double max) : min_(min), max_(max)
{
}

// at init dof are at min value
void Driver::start()
{
    values_[0] = min_;
    values_[1] = min_;
}

// just clip desired values
// between 0 and 1000
void Driver::set(const DriverIn &in)
{
    for (unsigned int i = 0; i < 2; i++)
    {
        if (in.values[i] > max_)
        {
	  values_[i] = max_;
        }
        else if (in.values[i] < min_)
        {
	  values_[i] = min_;
        }
        else
        {
	  values_[i] = in.values[i];
        }
    }
}

DriverOut Driver::get()
{
  DriverOut out;
  out.values[0] = values_[0];
  out.values[1] = values_[1];
  return out;
}

  void Driver::stop(){}

}
