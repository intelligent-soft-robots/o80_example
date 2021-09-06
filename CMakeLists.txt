cmake_minimum_required(VERSION 3.10.2)

project(o80_example VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)


######################
# Compile Definition #
######################

# the following file:
# https://github.com/machines-in-motion/real_time_tools/blob/master/include/real_time_tools/thread.hpp
# needs either NON_REAL_TIME or RT_PREEMPT to be defined 
# "regular" OS
add_compile_definitions(NON_REAL_TIME)
# RT-Preempt OS
#add_compile_definitions(RT_PREEMPT)


################
# Dependencies #
################

find_package(Boost REQUIRED COMPONENTS filesystem system thread)


#o80
if ((DEFINED o80_DIR) AND (EXISTS ${o80_DIR}/include/o80) AND (EXISTS ${o80_DIR}/lib))
	set(o80 ${o80_DIR})
else()
	find_path(o80 PATHS ${o80_DIR} NAMES include/o80 lib/libo80.so)
endif()
if (NOT "${o80}" MATCHES "o80-NOTFOUND")
	message("-- o80 FOUND. o80 at ${o80}/lib/libo80.so")
	set(o80_INCLUDE_DIRS ${o80}/include)
	set(o80_LIBRARY_DIRS ${o80}/lib)
	set(o80_LIBRARIES real_time_tools shared_memory signal_handler synchronizer time_series o80)
else()
	message(FATAL_ERROR
	"  Could not find an C++ include file \"include/o80/front_end.hpp\"\n"
	"  Make sure \"o80\" is installed or set \"o80_DIR\" to a directory\n"
	"  containing \"include\" and \"lib\" subdirectories of \"o80\" project.")
endif()

#pybind11

find_package(Python COMPONENTS Interpreter Development REQUIRED)

find_package(pybind11 CONFIG REQUIRED)
if(pybind11_FOUND)
	exec_program(python3 ARGS -m pybind11 --includes | cut -d ' ' -f1 | sed 's+-I/+/+g' OUTPUT_VARIABLE pybind11_INCLUDE_DIR1)
	exec_program(python3 ARGS -m pybind11 --includes | cut -d ' ' -f2 | sed 's+-I/+/+g' OUTPUT_VARIABLE pybind11_INCLUDE_DIR2)
	exec_program(python3 ARGS --version | cut -d ' ' -f2 | cut -d '.' -f1 OUTPUT_VARIABLE python_VERSION_MAJOR)
	exec_program(python3 ARGS --version | cut -d ' ' -f2 | cut -d '.' -f2 OUTPUT_VARIABLE python_VERSION_MINOR)
	set(pybind11_INCLUDE_DIRS ${pybind11_INCLUDE_DIR1} ${pybind11_INCLUDE_DIR2})
	unset(pybind11_INCLUDE_DIR1)
	unset(pybind11_INCLUDE_DIR2)
endif()

# eigen
find_package(Eigen3 REQUIRED)


###########
# Library #
###########

add_library( ${PROJECT_NAME} SHARED
  src/driver_in.cpp
  src/driver_out.cpp
  src/standalone.cpp
  src/driver.cpp )

target_include_directories(o80_example PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_include_directories(o80_example PUBLIC ${o80_INCLUDE_DIRS})
target_include_directories(o80_example PUBLIC ${EIGEN3_INCLUDE_DIRS})
target_include_directories(o80_example PUBLIC ${Python_INCLUDE_DIRS})
target_link_directories(o80_example PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}> $<INSTALL_INTERFACE:lib>)
target_link_directories(o80_example PUBLIC ${o80_LIBRARY_DIRS})
target_link_libraries(o80_example PUBLIC ${o80_LIBRARIES})
target_link_libraries(o80_example PUBLIC boost_system boost_thread rt pthread)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_SOURCE_DIR}/o80_example-config-version.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)
export(TARGETS o80_example FILE "${PROJECT_SOURCE_DIR}/o80_example-config.cmake")
install(DIRECTORY include/o80_example TYPE INCLUDE)
install(TARGETS o80_example EXPORT o80_example-config)
install(EXPORT o80_example-config DESTINATION lib/cmake/o80_example)
install(FILES "${PROJECT_SOURCE_DIR}/o80_example-config-version.cmake" DESTINATION lib/cmake/o80_example)




###################
# Python wrappers #
###################


# o80_example_python
if (pybind11_FOUND)
  add_library(o80_example_python SHARED srcpy/wrappers.cpp)
  if (o80_example_omit_include_directories MATCHES yes)
    target_compile_definitions(o80_example_python PRIVATE O80_EXAMPLE_WRAPPERS_INSTALLED)
  endif()
  target_include_directories(o80_example_python PRIVATE ${pybind11_INCLUDE_DIRS})
  target_link_libraries(o80_example_python PRIVATE o80_example)
  set_target_properties(o80_example_python PROPERTIES PREFIX "")
  set_target_properties(o80_example_python PROPERTIES OUTPUT_NAME "o80_example.cpython-${python_VERSION_MAJOR}${python_VERSION_MINOR}-${CMAKE_LIBRARY_ARCHITECTURE}")
  install(TARGETS o80_example_python DESTINATION "lib/python${python_VERSION_MAJOR}.${python_VERSION_MINOR}/dist-packages")
endif()


