#include "o80/pybind11_helper.hpp"
#include "o80_example/standalone.hpp"

PYBIND11_MODULE(o80_example_wrp, m)
{
    o80::create_python_bindings<o80_example::Standalone>(m);
    o80::create_standalone_python_bindings<o80_example::Driver,
                                           o80_example::Standalone,
                                           double,
                                           double>  // double,double are
                                                    // arguments to drivers
        (m);
}
