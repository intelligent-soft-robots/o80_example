# Running the demos

This section provides instructions on how to run the demos. Details on what these demos does are explained in following sections. 


---
These demos may also be run on docker container based on an image available on [docker-hub](https://hub.docker.com/repository/docker/vincentberenz/o80).
See the last section of this page for more information.
---

## sourcing the workspace

In all new terminal, the workspace must be sourced.

```bash
cd Software
cd workspace
source ./devel/setup.bash
```

You may find it convenient to source the workspace in the ~/.bashr file.

## running the non bursting mode demos

### starting the o80 standalone

After sourcing the workspace, you may start a standalone:

```bash
cd Software
cd workspace
source ./devel/setup.bash
cd src/o80_example/demos
python3 ./standalone_backend.py
```

A plot should appears. At the bottom, the frequency of the standalone is plotted. At the top, the values of the states of the robot are plotted (they should be at 0 for now).

### running demos

In  another terminal, run one of the demo, e.g. "duration_commands".


```bash
cd Software
cd workspace
source ./devel/setup.bash
cd src/o80_example/demos
python3 ./duration_commands.py
```

You should see the state of the robot change. You may thereafter run other demos (all python files with the exception of "bursting.py" and "standalone_backend_bursting.py").

## running the bursting demo

In the bursting mode, the standalone does not run at a specified frequency, rather, it execute iterations upon frontends requests.

### starting a bursting standalone

```bash
cd Software
cd workspace
source ./devel/setup.bash
cd src/o80_example/demos
python3 ./standalone_backend_bursting.py
```

You should see a plot, showing the state values of the robot (now at 0).

### running the bursting demo

In another terminal:

```bash
cd Software
cd workspace
source ./devel/setup.bash
cd src/o80_example/demos
python3 ./bursting.py
```

The plot should show the state values change. 


# Running the demos from docker containers

The docker images vincentberenz/o80 provides an installation of ubuntu 18.04 with a compile catkin o80 workspace.
You may use it for running the demos.

This section assumes you are familiar with (docker)[https://www.docker.com/].

## download the image

```bash
docker pull vincentberenz/o80:v1.0
```

## start a container that will run the backend

The container must share between the host and the container:
- the shared memory folder (/dev/shm) 
- the display

For example:

```bash
docker run -it \
    -v /dev/shm:/dev/shm \
    --env="DISPLAY" \
    --env="QT_X11_NO_MITSHM=1" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    -v /tmp:/tmp docker.is.localnet:5000/amd/18.04:o80 /bin/bash
```

For alternative methods for sharing the display, see this [tutorial](http://wiki.ros.org/docker/Tutorials/GUI).

Once the container started, you may start an o80 backend:

```bash
python /opt/o80/workspace/src/o80_example/demos/standalone_backend.py
```

A window should appear displaying two plots (joint states and backend frequency).

### run a demo

In another terminal, start another container (same command as above). You may then run the demos,
for example:

```bash
python /opt/o80/workspace/src/o80_example/demos/duration_commands.py
```