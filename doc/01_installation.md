
# Installation

o80_example has been tested on Ubuntu 18.04 / c++17 / python3.
It uses [catkin tools](https://catkin-tools.readthedocs.io) for compilation.
It requires several catkin packages, and uses [treep](https://pypi.org/project/treep/) as a project manager.

---
You may also run the o80 demos using a docker installation.
See the [demos](https://intelligent-soft-robots.github.io/code_documentation/o80_example/docs/html/doc/02_demos.html) documentation page.
---

## Overview : catkin tools

We use catkin to build o80 and its related packages. Catkin is the build tool of [ROS](https://www.ros.org/),
so if you worked with robots before, chances are good that you are already familiar with it.
If not, please see [Creating a ROS package](wiki.ros.org/catkin/Tutorials/CreatingPackage).

We are using [catkin_tools](https://catkin-tools.readthedocs.io/en/latest/index.html) and build with Python 3
(i.e. the python bindings will be created for python3).

The typical workflow of working with catkin tools:

```bash
# creating a workspace
mkdir -p workspace/src
cd workspace/src
# cloning packages in the workspace
git clone <git url of package 1>
git clone <git url of package 2>
# ...
# back to the workspace folder
cd .. 
# compiling, this above created the folder 'devel'
catkin build 
# 'sourcing' the workspace: this upgrades 
# environmont variables such as 
# $PATH, $PYTHONPATH, $LD_LIBRARY path
source ./devel/setup.bash
```
The installation instructions below are based on a similar workflow.

## Overview: treep

In a typical catkin workflow, more than one repository needs to be cloned in the workspace.
While it is possible to clone these repositories one by one, the installation procedure below uses
treep to clone all the repos via a single command. A typical treep workflow is:

- clone a treep configuration folder
- call ```treep --clone PROJECT_NAME```

The installation instructions below provides the git url to the correct treep configuration folder.

## Install the dependencies

Here are the required dependencies for o80 (including catkin and treep).

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

This will create a folder "workspace" and clone various catkin packages in it (including o80 and o80_example)

## Compilation

[catkin tools](https://catkin-tools.readthedocs.io/) is used for compilation:

### if python3 is the default python (or you are using a python3 virtual environment):

(you may call ```python --version``` to check your python version).

```bash
catkin build

```
### if python3 is not the default python:

```bash
catkin config --cmake-args -DPYTHON_EXECUTABLE=/usr/bin/python3
catkin build
```

