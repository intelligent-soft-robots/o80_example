#pragma once

#include <iostream>
#include <string>

namespace o80_example
{
class Action
{
public:
    void print(bool endl);
    std::string to_string() const;
    double values[2];
};
}