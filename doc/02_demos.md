# Running the demos

This section provides instructions on how to run the demos. Information on what these demos does is provided in following sections. 

## downloading the example repository

The git repository is [here](https://github.com/intelligent-soft-robots/o80_example.git).


## running the non bursting mode demos

### starting the o80 standalone

You may start a standalone:

```bash
cd o80_example/demos
python3 ./standalone_backend.py
```

A plot should appears. At the bottom, the frequency of the standalone is plotted. At the top, the values of the states of the robot are plotted (they should be at 0 for now).

### starting introspection (optional)

After starting the standalone, in another terminal:

```bash
cd o80_example/demos
python3 ./introspection.py
```

This will result in the runtime display of the activity of the frontend and the backend. 


### running demos

In  another terminal, run one of the demo, e.g. "duration_commands".


```bash
cd o80_example/demos
python3 ./duration_commands.py
```

You should see the state of the robot change. You may thereafter run other demos (all python files with the exception of "bursting.py" and "standalone_backend_bursting.py").

## running the bursting demo

In the bursting mode, the standalone does not run at a specified frequency, rather, it execute iterations upon frontends requests.

### starting a bursting standalone

```bash
cd o80_example/demos
python3 ./standalone_backend_bursting.py
```

You should see a plot, showing the state values of the robot (now at 0).

### running the bursting demo

In another terminal:

```bash
cd o80_example/demos
python3 ./bursting.py
```

The plot should show the state values change. 


