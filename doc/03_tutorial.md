
# Demos explained

## Overview

In the demo, a toy "robot" has 2 actuators/joints. Each actuator being characterized by a state which just encapsulates a double. o80 is used to send desired state commands (i.e. desired state values) to an instance of this robot.

## The source code

To integrate a robot with o80, the following classes must be implemented:

- a *Driver* class, which inherates o80::Driver, and templated over the user classes *DriverIn* and *DriverOut*. The driver class will be used by o80 for communicating with the robot.

- a *State* class, possibly inherating from o80::State. This represents the state of an actuator.

- an optional ExtendedState class, which encapsulates any robot sensory information that is not an actuator State.

- a *Standalone* class, inherating from o80::Standalone, and templated over the Driver, the State and the ExtendedState.

For these example, these classes have been implemented:

### Driver

The driver implements the code of the toy robot. Here the sources (code is trivial):

- [driver.hpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/include/o80_example/driver.hpp)

- [driver.cpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/src/driver.cpp)

- [driver_in.hpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/include/o80_example/driver_in.hpp)

- [driver_in.cpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/src/driver_in.cpp)

- [driver_out.hpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/include/o80_example/driver_out.hpp)

- [driver_out.cpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/src/driver_out.cpp)


o80 will call at runtime the set method of the driver to input values of desired states (one value per actuator) and use the get method to get sensor values (here limited to the observed states of the two actuators)

### State

An instance of the Joint class encapsulates the state of one actuator of the robot.

- [joint.hpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/include/o80_example/joint.hpp)

- [joint.cpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/src/joint.cpp)

The code of Joint implements methods that specify how to interpolate from one desired state to another. These function will be called by the standalone when consuming for example "duration" commands (i.e. the standalone needs to interpolate from the current state value to the target state value over a specified duration). In this case, these methods are implemented in the [o80::State](https://github.com/intelligent-soft-robots/o80/blob/master/include/o80/state.hpp#L46) super class. The methods of the superclass supports only native type states (here a state is a double) with linear interpolation. For more complex states or interpolation method, the "intermediate_state" methods must be overriden.

During runtime, instances of various classes will be serialized into the shared memory. o80 uses [cereal](https://uscilab.github.io/cereal/) for serialization. The State class used must implement a [serialize method](https://github.com/intelligent-soft-robots/o80/blob/master/include/o80/state.hpp#L74), which is done here in the super class of Joint.


### Extended State

The standalone get robot sensors values via the "get" method of the driver. From the robot sensor values the observed state of the robot actuators are extracted via the Standalone "convert" method. Extra informations (i.e. any robot sensor information that is not related to state) can be extracted using the "enrich_extended_state" method of Standalone, and encapsulated in an instance of ExtendedState (arbitrary user defined class). In this case, there is no extra information, hence no extended state is used, and the Standalone is templated using [o80::VoidExtendedState](https://github.com/intelligent-soft-robots/o80/blob/master/include/o80/void_extended_state.hpp).

Similarly to State, the ExtendedState class must implement a serialize method.

### Standalone

An instance of Standalone is a "bridge" between the robot driver and the frontend. 

- [standalone.hpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/include/o80_example/standalone.hpp)

- [standalone.cpp](https://github.com/intelligent-soft-robots/o80_example/blob/master/src/standalone.cpp)

The Standalone class inherates from o80::Standalone, templated over Driver, Joint and VoidExtendedState. Two more templates are provided:

- QUEUE_SIZE (int): the size of the shared memories that will be used for exchanging commands and observations between frontends and backend. The bigger the number:

    - the more commands can be shared, i.e. full trajectories over a higher number of iterations can be specified
    - the bigger the available observation history (via o80::FrontEnd::get_latest_observations methods)

- NB_DOFS (int): the number of actuators, here two.

The standalone must implement two methods:

```cpp
DriverIn convert(const o80::States<2, Joint> &joints);
```

At runtime, o80 will call this method to "convert" desired joint values (as specified by the commands sent by the frontend(s)) to instances of DriverIn, i.e. the input suitable for the robot driver.

```cpp
void enrich_extended_state(o80::VoidExtendedState &extended_state,
                               const DriverOut &driver_out);
```

At runtime, o80 will call this method to "fill" the extended_state with robot sensor values.


## The python bindings

[pybind11](https://pybind11.readthedocs.io) is used to create python bindings, and o80 provides [higher level functions](https://github.com/intelligent-soft-robots/o80/blob/master/include/o80/pybind11_helper.hpp) to smooth up things. 

The python wrappers are defined and declared there: [o80_example/srcpy/wrappers](https://github.com/intelligent-soft-robots/o80_example/blob/master/srcpy/wrappers.cpp).

```cpp
PYBIND11_MODULE(o80_example, m)
{
    o80::create_python_bindings<o80_example::Driver,
                                o80_example::Standalone,
                                double,
                                double>  // int,int are arguments to drivers
		            (m);
}
```

The "create_python_bindings" will create bindings over all o80 classes, including FrontEnd, BackEnd, Standalone, State, etc.

The CMakeLists is updated to create the [o80_example python package](https://github.com/intelligent-soft-robots/o80_example/blob/master/CMakeLists.txt#L41) at compile time.

Once the workspace sourced, the "o80_example" python package is available in the path.

```python
import o80_example
```

See the source code of the demos for usage.

