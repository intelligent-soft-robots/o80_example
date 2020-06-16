#include "o80_example/driver_in.hpp"

namespace o80_example
{
void DriverIn::print(bool endl)
{
    std::cout << "driver in: " << values[0] << "\t" << values[1] << " ";
    if (endl) std::cout << std::endl;
}

std::string DriverIn::to_string() const
{
    return std::to_string(values[0]) + " " + std::to_string(values[1]);
}

double values[2];
}
