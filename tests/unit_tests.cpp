#include <unistd.h>
#include <cstdlib>
#include <set>
#include <sstream>
#include "gtest/gtest.h"

#include "o80/back_end.hpp"
#include "o80/burster.hpp"
#include "o80/command_types.hpp"
#include "o80/front_end.hpp"
#include "o80/observation.hpp"
#include "o80/sensor_state.hpp"
#include "o80/state2d.hpp"
#include "o80/state3d.hpp"
#include "o80_example/driver.hpp"
#include "o80_example/driver_in.hpp"
#include "o80_example/driver_out.hpp"
#include "o80_example/joint.hpp"
#include "o80_example/joint2d.hpp"
#include "o80_example/standalone.hpp"

#include "real_time_tools/thread.hpp"

using namespace o80;

static inline void clear_memory()
{
    shared_memory::clear_shared_memory(o80_EXAMPLE_SEGMENT);
}

// class for testing sensor states
class SensorStateTest : public o80::SensorState
{
public:
  int value;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(value);
  }
};

class o80_tests : public ::testing::Test
{
    void SetUp()
    {
        clear_memory();
        shared_memory::get_segment_mutex(o80_EXAMPLE_SEGMENT).unlock();
    }
    void TearDown()
    {
        clear_memory();
    }
};

TEST_F(o80_tests, statexd_set_get)
{
    o80_example::Joint2d state(10.0, 20);

    ASSERT_EQ(state.get<0>(), 10.0);
    ASSERT_EQ(state.get<1>(), 20);

    state.set<0>(100.0);
    state.set<1>(200);

    ASSERT_EQ(state.get<0>(), 100.0);
    ASSERT_EQ(state.get<1>(), 200);
}

TEST_F(o80_tests, statexd)
{
    o80_example::Joint2d start_state(0, 0);
    o80_example::Joint2d current_state(50, 50);
    o80_example::Joint2d previous_desired_state(50, 50);
    o80_example::Joint2d target_state(100, 100);

    o80::TimePoint start(0);
    o80::Speed speed = o80::Speed::per_second(50);

    {
        o80::TimePoint now(static_cast<long int>(1e9));
        o80_example::Joint2d interpolated_state;
        interpolated_state =
            interpolated_state.intermediate_state(start,
                                                  now,
                                                  start_state,
                                                  current_state,
                                                  previous_desired_state,
                                                  target_state,
                                                  speed);
        bool finished = interpolated_state.finished(start,
                                                    now,
                                                    start_state,
                                                    current_state,
                                                    previous_desired_state,
                                                    target_state,
                                                    speed);

        ASSERT_EQ(interpolated_state.get<0>(), 50.0);
        ASSERT_EQ(interpolated_state.get<1>(), 50);
        ASSERT_EQ(finished, false);
    }

    {
        o80::TimePoint now(static_cast<long int>(1.5e9));
        o80_example::Joint2d interpolated_state;
        interpolated_state =
            interpolated_state.intermediate_state(start,
                                                  now,
                                                  start_state,
                                                  current_state,
                                                  previous_desired_state,
                                                  target_state,
                                                  speed);
        bool finished = interpolated_state.finished(start,
                                                    now,
                                                    start_state,
                                                    current_state,
                                                    previous_desired_state,
                                                    target_state,
                                                    speed);

        ASSERT_EQ(interpolated_state.get<0>(), 75.0);
        ASSERT_EQ(interpolated_state.get<1>(), 75);
        ASSERT_EQ(finished, false);
    }

    {
        o80::TimePoint now(static_cast<long int>(2e9));
        o80_example::Joint2d interpolated_state;
        interpolated_state =
            interpolated_state.intermediate_state(start,
                                                  now,
                                                  start_state,
                                                  current_state,
                                                  previous_desired_state,
                                                  target_state,
                                                  speed);
        bool finished = interpolated_state.finished(start,
                                                    now,
                                                    start_state,
                                                    current_state,
                                                    previous_desired_state,
                                                    target_state,
                                                    speed);

        ASSERT_EQ(interpolated_state.get<0>(), 100.0);
        ASSERT_EQ(interpolated_state.get<1>(), 100);
        ASSERT_EQ(finished, false);
    }

    {
        o80::TimePoint now(static_cast<long int>(3e9));
        o80_example::Joint2d interpolated_state;
        interpolated_state =
            interpolated_state.intermediate_state(start,
                                                  now,
                                                  start_state,
                                                  current_state,
                                                  previous_desired_state,
                                                  target_state,
                                                  speed);
        bool finished = interpolated_state.finished(start,
                                                    now,
                                                    start_state,
                                                    current_state,
                                                    previous_desired_state,
                                                    target_state,
                                                    speed);

        ASSERT_EQ(interpolated_state.get<0>(), 100.0);
        ASSERT_EQ(interpolated_state.get<1>(), 100);
        ASSERT_EQ(finished, true);
    }
}

TEST_F(o80_tests, statexd_serialization)
{
    o80_example::Joint2d in(12.4, 5);

    shared_memory::Serializer<o80_example::Joint2d> serializer;
    const std::string& data = serializer.serialize(in);

    o80_example::Joint2d out;
    serializer.deserialize(data, out);

    ASSERT_EQ(out.get<0>(), 12.4);
    ASSERT_EQ(out.get<1>(), 5);
}

TEST_F(o80_tests, state2d_and_3d)
{
    o80::State2d state2d(1.0, 2.0);
    o80::State3d state3d(1.0, 2.0, 3.0);

    ASSERT_EQ(state2d.get<1>(), 2.0);
    ASSERT_EQ(state3d.get<2>(), 3.0);
}

TEST_F(o80_tests, observation_serialization)
{
  typedef o80::Observation<2,o80_example::Joint,o80::VoidExtendedState> Obs;
  typedef o80::States<2,o80_example::Joint> Sta;
  long int stamp = o80::time_now().count();
  long int iteration = 5;
  double frequency = 200.;
  Sta sta;
  Obs o1(sta,sta,stamp,iteration,frequency);
  shared_memory::Serializer<Obs> serializer;
  const std::string& data = serializer.serialize(o1);
  Obs o2;
  serializer.deserialize(data, o2);
  ASSERT_EQ(o1.get_time_stamp(),stamp);
  ASSERT_EQ(o2.get_time_stamp(),stamp);
  ASSERT_EQ(o1.get_frequency(),frequency);
  ASSERT_EQ(o2.get_frequency(),frequency);
  ASSERT_EQ(o1.get_iteration(),iteration);
  ASSERT_EQ(o2.get_iteration(),iteration);
}

TEST_F(o80_tests, backend_sensor_state)
{
    clear_shared_memory("backend_sensor_state");
    BackEnd<1000, 2, SensorStateTest, o80::VoidExtendedState> backend(
        "backend_sensor_state");
}

// check return false on too many commands
TEST_F(o80_tests, command_status)
{
    CommandStatus<o80_example::Joint> status;

    o80_example::Joint starting_state(200);
    o80_example::Joint target_state(100);
    TimePoint start_time(10000);
    int current_iteration = 1001;
    TimePoint now(100001);
    o80_example::Joint current_state(150);

    bool thrown = false;
    try
    {
        status.finished(current_iteration,
                        now,
                        starting_state,
                        current_state,
                        current_state,
                        target_state);
    }
    catch (...)
    {
        thrown = true;
    }

    // because has not been initialized
    ASSERT_EQ(thrown, true);

    ASSERT_EQ(status.is_active(), false);

    status.set_active();

    ASSERT_EQ(status.is_active(), true);
}

TEST_F(o80_tests, direct_command_status)
{
    CommandStatus<o80_example::Joint> status;

    o80_example::Joint starting_state(200);
    o80_example::Joint target_state(100);
    int starting_iteration = 1000;
    TimePoint start_time(10000);

    status.set_initial_conditions(starting_iteration,
                                  starting_state,
                                  target_state,
                                  start_time,
                                  CommandType());
    int current_iteration = 1001;
    TimePoint now(100001);
    o80_example::Joint current_state(150);
    bool ended = status.finished(current_iteration,
                                 now,
                                 starting_state,
                                 current_state,
                                 current_state,
                                 target_state);

    ASSERT_EQ(ended, false);

    status.set_direct_done();

    ended = status.finished(current_iteration,
                            now,
                            starting_state,
                            current_state,
                            current_state,
                            target_state);

    ASSERT_EQ(ended, true);
}

TEST_F(o80_tests, iteration_command_status)
{
    CommandStatus<o80_example::Joint> status;

    o80_example::Joint starting_state(200);
    o80_example::Joint target_state(100);
    int starting_iteration = 1000;
    int target_iteration = 2000;
    TimePoint start_time(10000);

    status.set_initial_conditions(starting_iteration,
                                  starting_state,
                                  target_state,
                                  start_time,
                                  CommandType(Iteration(target_iteration)));

    TimePoint now(100001);
    o80_example::Joint current_state(150);

    for (int current_iteration = 0; current_iteration < 2000;
         current_iteration++)
    {
        bool ended = status.finished(current_iteration,
                                     now,
                                     starting_state,
                                     current_state,
                                     current_state,
                                     target_state);
        ASSERT_EQ(ended, false);
    }

    bool ended = status.finished(
        2001, now, starting_state, current_state, current_state, target_state);
    ASSERT_EQ(ended, true);
}

TEST_F(o80_tests, controllers_manager)
{
    std::string segment_id("ut_controllers_manager");
    o80::clear_shared_memory(segment_id);

    typedef ControllersManager<2, o80_EXAMPLE_QUEUE_SIZE, o80_example::Joint>
        CM;

    CM manager(segment_id);
    CM::CommandsTimeSeries& commands = manager.get_commands_time_series();
    CM::CompletedCommandsTimeSeries& completed_commands =
        manager.get_completed_commands_time_series();

    // initializing both joints to 0

    o80_example::Joint d0(0);
    o80_example::Joint d1(0);

    manager.get_desired_state(0, 0, TimePoint(0), d0);
    manager.get_desired_state(1, 0, TimePoint(0), d1);

    // commands for dof 0
    // goes to 100 in 10 iterations then to
    // 200 also in 10 iterations

    Command<o80_example::Joint> c00(
        1, o80_example::Joint(100), Iteration(100), 0, Mode::QUEUE);
    Command<o80_example::Joint> c01(
        1, o80_example::Joint(200), Iteration(200), 0, Mode::QUEUE);

    // commands for dof1
    // first command is cancelled by the overwrite mode
    // of the second command
    // Only the second command is executed,
    // i.e. direct command of 500

    Command<o80_example::Joint> c10(
        1, o80_example::Joint(100), Iteration(100), 1, Mode::QUEUE);
    Command<o80_example::Joint> c11(
        1, o80_example::Joint(500), 1, Mode::OVERWRITE);

    // sending commands to the manager
    commands.append(c00);
    commands.append(c01);
    commands.append(c10);
    commands.append(c11);
    shared_memory::set<long int>(segment_id, "pulse_id", 1);

    // processing the commands
    manager.process_commands(0);

    // saving the ids of all commands

    std::array<int, 4> ids = {
        c00.get_id(), c01.get_id(), c10.get_id(), c11.get_id()};

    // desired states after 50 iterations

    int iter = 0;

    for (; iter <= 50; iter++)
    {
        d0 = manager.get_desired_state(0, iter, TimePoint(0), d0);
        d1 = manager.get_desired_state(1, iter, TimePoint(0), d1);
    }

    // current command for dof 0 is first command
    // current command for dof 1 is -1 (no running command,
    // one was overwritten, the other finished)

    ASSERT_EQ(manager.get_current_command_id(0), c00.get_id());
    ASSERT_EQ(manager.get_current_command_id(1), -1);

    // d0 expected between 0(init) and 100 (end of first command)

    ASSERT_GT(d0.get(), 0);
    ASSERT_LT(d0.get(), 100);

    // d1 expected 500 (direct command of 500)

    ASSERT_EQ(d1.get(), 500);

    // desired state after 150 iterations

    for (; iter <= 152; iter++)
    {
        d0 = manager.get_desired_state(0, iter, TimePoint(0), d0);
        d1 = manager.get_desired_state(1, iter, TimePoint(0), d1);
    }

    // command for dof0 and dof1 is second command

    ASSERT_EQ(manager.get_current_command_id(0), c01.get_id());
    ASSERT_EQ(manager.get_current_command_id(1), -1);

    // dof 0 expected between 100 and 200 (middle of second command)
    // dof 1 still at 500

    ASSERT_GT(d0.get(), 100);
    ASSERT_LT(d0.get(), 200);
    ASSERT_EQ(d1.get(), 500);

    // desired state after 250 iterations (all commands terminated)

    for (; iter <= 250; iter++)
    {
        d0 = manager.get_desired_state(0, iter, TimePoint(0), d0);
        d1 = manager.get_desired_state(1, iter, TimePoint(0), d1);
    }

    ASSERT_EQ(manager.get_current_command_id(0), -1);
    ASSERT_EQ(manager.get_current_command_id(1), -1);

    // final desired states of last commands

    ASSERT_EQ(d0.get(), 200);
    ASSERT_EQ(d1.get(), 500);

    // checking the manager stored the ids of executed commands

    std::set<int> executed_commands;
    time_series::Index oldest = completed_commands.oldest_timeindex();
    time_series::Index newest = completed_commands.newest_timeindex();
    for (time_series::Index index = oldest; index <= newest; index++)
    {
        executed_commands.insert(completed_commands[index]);
    }

    ASSERT_EQ(executed_commands.size(), (size_t)4);

    for (int id : ids)
    {
        auto f = executed_commands.find(id);
        ASSERT_TRUE(f != executed_commands.end());
    }
}

TEST_F(o80_tests, front_and_backends_basic)
{
    // in this tests we have the frontends adding commands,
    // we spin the backend so that it executes them,
    // and we check using the frontend that the expected
    // desired state are indeed produced.

    clear_shared_memory("f_a_bends_utests");

    BackEnd<1000, 2, o80_example::Joint, o80::VoidExtendedState> backend(
        "f_a_bends_utests");
    FrontEnd<1000, 2, o80_example::Joint, o80::VoidExtendedState> frontend(
        "f_a_bends_utests");

    o80_example::Joint j0(0);
    o80_example::Joint j1(0);

    frontend.add_command(
        0, o80_example::Joint(100), Iteration(100), Mode::QUEUE);
    frontend.add_command(
        0, o80_example::Joint(200), Iteration(200), Mode::QUEUE);

    frontend.add_command(
        1, o80_example::Joint(100), Iteration(100), Mode::QUEUE);

    frontend.add_command(1, o80_example::Joint(100), Mode::OVERWRITE);
    frontend.add_command(
        1, o80_example::Joint(300), Iteration(300), Mode::QUEUE);

    frontend.pulse();

    States<2, o80_example::Joint> states;
    states.set(0, j0);
    states.set(1, j1);

    for (int iter = 0; iter <= 100; iter++)
    {
        states = backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    }

    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.read();
    states = observation.get_desired_states();
    long int iteration = observation.get_iteration();

    j0 = states.get(0);
    j1 = states.get(1);

    ASSERT_EQ(iteration, 100);
    ASSERT_EQ(j0.value, 100);
    ASSERT_GT(j1.value, 100);
    ASSERT_LT(j1.value, 300);

    frontend.add_command(
        1, o80_example::Joint(400), Iteration(400), Mode::QUEUE);

    for (int iter = 101; iter <= 300; iter++)
    {
        states = backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    }

    observation = frontend.pulse();
    states = observation.get_desired_states();

    j0 = states.get(0);
    j1 = states.get(1);

    ASSERT_EQ(j0.value, 200);
    ASSERT_EQ(j1.value, 300);

    for (int iter = 301; iter <= 400; iter++)
    {
        states = backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    }

    observation = frontend.read();
    states = observation.get_desired_states();

    j0 = states.get(0);
    j1 = states.get(1);

    ASSERT_EQ(j0.value, 200);
    ASSERT_EQ(j1.value, 400);
}

TEST_F(o80_tests, front_and_backends_reapply)
{
    clear_shared_memory("f_a_bends_utests");

    bool new_commands_observations = true;  // !
    BackEnd<1000, 2, o80_example::Joint, o80::VoidExtendedState> backend(
        "f_a_bends_utests", new_commands_observations);
    FrontEnd<1000, 2, o80_example::Joint, o80::VoidExtendedState> frontend(
        "f_a_bends_utests");

    long int iteration = -1;

    frontend.add_command(0, o80_example::Joint(1), Mode::QUEUE);
    frontend.pulse();

    States<2, o80_example::Joint> states;
    backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.pulse();
    long int iteration2 = observation.get_iteration();

    ASSERT_NE(iteration, iteration2);

    backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    observation = frontend.pulse();
    long int iteration3 = observation.get_iteration();

    ASSERT_EQ(iteration2, iteration3);

    frontend.add_command(0, o80_example::Joint(1), Mode::QUEUE);
    frontend.pulse();
    backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
    observation = frontend.pulse();
    long int iteration4 = observation.get_iteration();

    ASSERT_NE(iteration3, iteration4);
}

static std::atomic<bool> RUNNING(true);

static void* frontend_wait_fn(void*)
{
    BackEnd<o80_EXAMPLE_QUEUE_SIZE,
            o80_EXAMPLE_NB_DOFS,
            o80_example::Joint,
            o80::VoidExtendedState>
        backend("frontend_wait_utests");

    States<o80_EXAMPLE_NB_DOFS, o80_example::Joint> states;
    o80_example::Joint j0(0);
    o80_example::Joint j1(0);
    states.set(0, j0);
    states.set(1, j1);

    while (RUNNING)
    {
        states = backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
        usleep(10);
    }
    
    return nullptr;
}

TEST_F(o80_tests, frontend_wait)
{
    // a backend runs in a separate threads.
    // a frontend sends command, and
    // wait for their completion. Then, checks
    // that indeed the desired of the robot
    // is as expected

    RUNNING = true;
    clear_shared_memory("frontend_wait_utests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(frontend_wait_fn);
    usleep(100000);

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("frontend_wait_utests");

    frontend.add_command(
        0, o80_example::Joint(100), Iteration(100), Mode::QUEUE);
    frontend.add_command(
        0, o80_example::Joint(200), Iteration(200), Mode::QUEUE);

    frontend.add_command(
        1, o80_example::Joint(100), Iteration(100), Mode::QUEUE);

    frontend.add_command(1, o80_example::Joint(100), Mode::OVERWRITE);
    frontend.add_command(
        1, o80_example::Joint(300), Iteration(300), Mode::QUEUE);

    TimePoint start = time_now();
    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.pulse_and_wait();
    TimePoint end = time_now();

    States<2, o80_example::Joint> states = observation.get_desired_states();
    o80_example::Joint j0 = states.get(0);
    o80_example::Joint j1 = states.get(1);

    RUNNING = false;
    thread.join();

    // false would mean did not wait
    long int duration = time_diff(start, end);
    ASSERT_GT(duration, 1000);

    ASSERT_EQ(j0.value, 200);
    ASSERT_EQ(j1.value, 300);
}

static void* frontend_wait_low_freq_fn(void*)
{
    BackEnd<o80_EXAMPLE_QUEUE_SIZE,
            o80_EXAMPLE_NB_DOFS,
            o80_example::Joint,
            o80::VoidExtendedState>
        backend("frontend_wait_utests");

    States<o80_EXAMPLE_NB_DOFS, o80_example::Joint> states;
    o80_example::Joint j0(0);
    o80_example::Joint j1(0);
    states.set(0, j0);
    states.set(1, j1);

    while (RUNNING)
    {
        states = backend.pulse(TimePoint(0), states, o80::VoidExtendedState());
        usleep(10000);
    }

    return nullptr;
}

TEST_F(o80_tests, frontend_wait_for_next)
{
    RUNNING = true;
    clear_shared_memory("frontend_wait_utests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(frontend_wait_low_freq_fn);
    usleep(50000);

    for (int a = 0; a < 3; a++)
    {
        FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
                 o80_EXAMPLE_NB_DOFS,
                 o80_example::Joint,
                 o80::VoidExtendedState>
            frontend("frontend_wait_utests");

        frontend.add_command(
            0, o80_example::Joint(100), Iteration(100), Mode::QUEUE);
        frontend.pulse();

        int iteration = -1;
        for (int i = 0; i < 20; i++)
        {
            Observation<2, o80_example::Joint, o80::VoidExtendedState>
                observation = frontend.wait_for_next();
            int iter = observation.get_iteration();
            if (iteration > 0)
            {
                ASSERT_EQ(iter, iteration + 1);
            }
            iteration = iter;
        }
    }

    RUNNING = false;
    thread.join();
}

TEST_F(o80_tests, standalone_destruction)
{
    std::string segment_id("ut_standalone_destruction");
    o80::clear_shared_memory(segment_id);
    std::shared_ptr<o80_example::Driver> driver_ptr(
        new o80_example::Driver(0, 1000));
    o80_example::Standalone standalone(driver_ptr, 100.0, segment_id);
}

TEST_F(o80_tests, standalone_runner)
{
    std::string segment_id("ut_standalone_runner");

    start_standalone<o80_example::Driver, o80_example::Standalone>(
        segment_id, 500, false, 0, 1000);
    usleep(2000);
    stop_standalone(segment_id);
}

static void* bursting_fn(void*)
{
    BackEnd<o80_EXAMPLE_QUEUE_SIZE,
            o80_EXAMPLE_NB_DOFS,
            o80_example::Joint,
            o80::VoidExtendedState>
        backend("ut_burster");

    o80::Burster burster("ut_burster");
    int iteration = 0;
    while (RUNNING)
    {
        shared_memory::set<int>("ut_burster", "nb_iterations", iteration);
        burster.pulse();
        iteration++;
    }

    return nullptr;
}

TEST_F(o80_tests, burster)
{
    o80::clear_shared_memory("ut_burster");
    o80::Burster::clear_memory("ut_burster");

    RUNNING = true;
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(bursting_fn);

    usleep(100000);

    shared_memory::set<int>("ut_burster", "nb_iterations", 0);
    o80::Burster::turn_on("ut_burster");
    int iteration;

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("ut_burster");

    frontend.burst(10);

    usleep(500000);

    shared_memory::get<int>("ut_burster", "nb_iterations", iteration);
    ASSERT_EQ(iteration, 10);

    usleep(500000);

    shared_memory::get<int>("ut_burster", "nb_iterations", iteration);
    ASSERT_EQ(iteration, 10);

    frontend.burst(15);

    shared_memory::get<int>("ut_burster", "nb_iterations", iteration);
    ASSERT_EQ(iteration, 25);

    o80::Burster::turn_off("ut_burster");
    frontend.final_burst();

    usleep(50000);

    shared_memory::get<int>("ut_burster", "nb_iterations", iteration);

    ASSERT_LT(27, iteration);

    RUNNING = false;
    thread.join();
}

static void* bursting_standalone_fn(void*)
{
    double frequency = 1000;
    std::shared_ptr<o80_example::Driver> driver =
        std::make_shared<o80_example::Driver>(0, 1000);
    o80_example::Standalone standalone(driver, frequency, "burst_unittests");

    standalone.start();

    bool bursting = true;

    while (RUNNING)
    {
        standalone.spin(bursting);
    }

    standalone.stop();

    return nullptr;
}

TEST_F(o80_tests, frontend_one_burst)
{
    RUNNING = true;
    clear_shared_memory("burst_unittests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(bursting_standalone_fn);

    usleep(500000);

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("burst_unittests");

    frontend.add_command(0, o80_example::Joint(100), Mode::QUEUE);
    frontend.add_command(
        1, o80_example::Joint(100), Iteration(100), Mode::QUEUE);

    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.burst(1);
    int iteration = observation.get_iteration();
    States<2, o80_example::Joint> states = observation.get_desired_states();
    o80_example::Joint j0 = states.get(0);
    o80_example::Joint j1 = states.get(1);

    ASSERT_EQ(iteration, 0);
    ASSERT_EQ(j0.value, 100.0);
    ASSERT_EQ(j1.value, 1.0);

    RUNNING = false;
    frontend.final_burst();
    thread.join();
}

TEST_F(o80_tests, relative_iteration_command)
{
    RUNNING = true;
    clear_shared_memory("burst_unittests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(bursting_standalone_fn);

    usleep(500000);

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("burst_unittests");

    frontend.add_command(
        1, o80_example::Joint(100), Iteration(99), Mode::QUEUE);

    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.burst(100);

    States<2, o80_example::Joint> states = observation.get_desired_states();
    o80_example::Joint j1 = states.get(1);

    ASSERT_EQ(observation.get_iteration(), 99);
    ASSERT_EQ(j1.value, 100);

    frontend.add_command(
        1, o80_example::Joint(200), Iteration(100, true, true), Mode::QUEUE);
    observation = frontend.burst(5);

    states = observation.get_desired_states();
    j1 = states.get(1);

    ASSERT_EQ(observation.get_iteration(), 104);
    ASSERT_EQ(j1.value, 105.);

    RUNNING = false;
    frontend.final_burst();
    thread.join();
}

TEST_F(o80_tests, frontend_burst)
{
    RUNNING = true;
    clear_shared_memory("burst_unittests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(bursting_standalone_fn);

    usleep(500000);

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("burst_unittests");
    frontend.add_command(
        0, o80_example::Joint(100), Iteration(100), Mode::QUEUE);
    frontend.add_command(
        0, o80_example::Joint(200), Iteration(200), Mode::QUEUE);

    frontend.add_command(
        1, o80_example::Joint(100), Iteration(100), Mode::QUEUE);

    // "overwrite" -> first command for dof 1
    // overwritten
    frontend.add_command(1, o80_example::Joint(0), Mode::OVERWRITE);
    frontend.add_command(1, o80_example::Joint(100), Mode::QUEUE);
    frontend.add_command(
        1, o80_example::Joint(300), Iteration(300), Mode::QUEUE);

    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation =
        frontend.burst(
            201);  // running 201 iteration, i.e. reaching iteration number 200
    int iteration = observation.get_iteration();

    States<2, o80_example::Joint> states = observation.get_desired_states();
    o80_example::Joint j0 = states.get(0);
    o80_example::Joint j1 = states.get(1);

    Observation<2, o80_example::Joint, o80::VoidExtendedState> observation2 =
        frontend.burst(150);  // running 150 extra iterations, i.e. reaching
                              // iteration number 350
    int iteration2 = observation2.get_iteration();

    States<2, o80_example::Joint> states2 = observation2.get_desired_states();
    o80_example::Joint j2 = states2.get(0);
    o80_example::Joint j3 = states2.get(1);

    ASSERT_EQ(iteration, 200);
    ASSERT_EQ(iteration2, 350);

    ASSERT_EQ(j0.value, 200);
    ASSERT_GT(j1.value, 100);
    ASSERT_LT(j1.value, 300);

    ASSERT_EQ(j2.value, 200);
    ASSERT_EQ(j3.value, 300);

    RUNNING = false;

    frontend.final_burst();

    thread.join();
}

TEST_F(o80_tests, history)
{
    RUNNING = true;
    clear_shared_memory("burst_unittests");
    real_time_tools::RealTimeThread thread;
    thread.create_realtime_thread(bursting_standalone_fn);

    usleep(500000);

    FrontEnd<o80_EXAMPLE_QUEUE_SIZE,
             o80_EXAMPLE_NB_DOFS,
             o80_example::Joint,
             o80::VoidExtendedState>
        frontend("burst_unittests");

    for (int i = 0; i < 100; i++)
    {
        frontend.add_command(0, o80_example::Joint(i), Mode::QUEUE);
    }
    frontend.burst(121);

    long int newest = frontend.read().get_iteration();
    ASSERT_EQ(newest, 120);  // iteration  number starting from 0

    typedef Observation<o80_EXAMPLE_NB_DOFS,
                        o80_example::Joint,
                        o80::VoidExtendedState>
        OBS;

    std::vector<OBS> history1 = frontend.get_observations_since(91);
    std::vector<OBS> history2 = frontend.get_latest_observations(30);
    ASSERT_EQ(history1.size(), (size_t)30);
    ASSERT_EQ(history2.size(), (size_t)30);

    std::array<std::vector<OBS>, 2> histories = {history1, history2};

    for (int i = 0; i < 9; i++)
    {
        for (const std::vector<OBS>& history : histories)
        {
            States<2, o80_example::Joint> states =
                history[i].get_desired_states();
            o80_example::Joint j = states.get(0);
            ASSERT_EQ(j.value, 91 + i);
        }
    }

    for (int i = 0; i < 20; i++)
    {
        for (const std::vector<OBS>& history : histories)
        {
            States<2, o80_example::Joint> states =
                history[i + 10].get_desired_states();
            o80_example::Joint j = states.get(0);
            ASSERT_EQ(j.value, 99);
        }
    }

    RUNNING = false;
    frontend.final_burst();

    thread.join();
}
