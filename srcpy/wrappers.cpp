#include "o80/pybind11_helper.hpp"
#include "o80_example/standalone.hpp"

PYBIND11_MODULE(o80_example, m)
{
    o80::create_python_bindings<o80_example::Driver,
                                o80_example::Standalone,
                                double,
                                double>  // int,int are arguments to drivers
        (m);
}
