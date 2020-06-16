cmake_minimum_required(VERSION 2.8.7)

project(o80_example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(catkin REQUIRED COMPONENTS o80)

# defined in catkin_package mpi_cmake_modules,
# used to detected for example if using real time computer,
# also host various cmake files
DEFINE_OS()

catkin_package(
 LIBRARIES  o80_example
 INCLUDE_DIRS include
 CATKIN_DEPENDS
 o80
)

include_directories(include
  example/include
  ${catkin_INCLUDE_DIRS} )


################
# Main Library #
################

add_library( o80_example
#  src/joint.cpp
  src/action.cpp
  src/observation.cpp
  src/standalone.cpp
  src/driver.cpp)
target_link_libraries(o80_example ${catkin_LIBRARIES})

###################################
# Python bindings for the example #
###################################

pybind11_add_module(o80_example_py srcpy/wrappers.cpp)
target_link_libraries(o80_example_py PRIVATE o80_example ${catkin_LIBRARIES})
set_target_properties(o80_example_py PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_PYTHON_DESTINATION}
  OUTPUT_NAME o80_example
  )
install(TARGETS o80_example_py DESTINATION ${CATKIN_GLOBAL_PYTHON_DESTINATION})


##############
# Unit tests #
##############

#
# Note : these tests servers as o80 general unit tests,
#        because o80 needs concrete instances to run
#

catkin_add_gtest(o80_unit_tests
  tests/main.cpp
  tests/unit_tests.cpp)
target_link_libraries(o80_unit_tests o80_example ${catkin_libraries})