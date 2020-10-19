
# Installation

o80_example has been tested on Ubuntu 18.04 / c++11 / python3.
It uses [catkin tools](https://catkin-tools.readthedocs.io) for compilation.
It requires several catkin packages, and uses [treep](https://pypi.org/project/treep/) as a project manager.


## Install the dependencies

Here are the required dependencies for o80.

That you may get using apt-get:

>> ssh, python3-pip, git, cmake, libcereal-dev, libboost-all-dev, libgtest-dev, libeigen3-dev, libedit-dev, libncurses5-dev, freeglut3-dev, libxmu-dev

That you may get using pip3:

>> catkin_tools, treep, fyplot, pyside2, empy

You may install these dependencies using this [script](https://github.com/intelligent-soft-robots/o80/blob/master/o80_dependencies)

o80 has other catkin packages as dependencies. The rest of the documentation below will provide the instructions on how to clone them (with treep) and compile them (with catkin tools).

## Add your ssh key to github

Treep, which will be used to clone all the required repositories, uses ssh, and therefore requires you to set your ssh key to github.

See: [github documentation](https://help.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh).

## Activate your ssh key

```bash
# replace id_rsa by your key file name
ssh-add ~/.ssh/id_rsa
```

## Clone treep configuration

Create a Software folder, get into it, and clone the treep configuration:

```bash
mkdir Software # you may use another folder name
cd Software
git clone git@github.com:intelligent-soft-robots/treep_isr.git
```

The treep configuration contains information about repositories to clone. See the files repositories.yaml and projects.yaml if you are curious.

## Clone all the required repositories

```bash
treep --clone O80
```

This will create a folder "workspace" and clone various catkin packges in it (including o80 and o80_example)

## Compilation

[catkin tools](https://catkin-tools.readthedocs.io/) is used for compilation:

```bash
# making sure python3, and not python2.7, is used
catkin config --cmake-args -DPYTHON_EXECUTABLE=/usr/bin/python3
# compiling
catkin build
```
