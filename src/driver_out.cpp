#include "o80_example/driver_out.hpp"

namespace o80_example
{
void DriverOut::print(bool endl)
{
    std::cout << "driver out: " << values[0] << "\t" << values[1];
    if (endl) std::cout << std::endl;
}
}
