#include "o80_example/observation.hpp"

namespace o80_example
{
void Observation::print(bool endl)
{
    std::cout << "observation: " << values[0] << "\t" << values[1];
    if (endl) std::cout << std::endl;
}
}
