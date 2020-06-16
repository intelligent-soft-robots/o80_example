#include "o80_example/action.hpp"

namespace o80_example
{
void Action::print(bool endl)
{
    std::cout << "action: " << values[0] << "\t" << values[1] << " ";
    if (endl) std::cout << std::endl;
}

std::string Action::to_string() const
{
    return std::to_string(values[0]) + " " + std::to_string(values[1]);
}

double values[2];
}
