#  Servo Controller & Hardware Simulator

A lightweight, github C++ program built to simulate, profile, and control multi-channel RC servos. 

This project provides real time state tracking, physical angle boundary clamping, safety emergency locking, and trajectory interpolation profiles without requiring external vendor hardware abstraction layers.


The software can Dynamically register, attach, and detach up to 4 concurrent servo motors with isolated data packets. it has Trajectory Interpolation, for example, it can run gradual frame-by-frame motor movement profiling via hardware cycle tick loops. it can also perform Automated Testing and Native sweep routines to cycle joints between angular constraints automatically.

Repository Architecture

- `ServoController.h`: System configuration constants, object definitions (`ServoMotor`), and core class APIs.  (def checkout cuz it sic!)


- `ServoController.cpp`: Implementation of movement loops, angle constraints, safety logic, and text dashboards.

- `main.cpp`: Multi-axis subsystem runtime test pipeline exercising hardware state machine baselines.

## How to Build and Run

## 1: Run directly on GitHub (I personally recommend)
1. At the top of this GitHub repository, click the green **Code** button.
2. Select the **Codespaces** tab, and click **Create codespace on main**.
3. Once the web-based IDE loads, open the built-in terminal at the bottom and run:
   ```bash
   g++ -std=c++11 main.cpp ServoController.cpp -o servo_sim
   ./servo_sim
   ```

## 2: Run Locally (I can't test it with my school issued chromebook)
Clone the repository and compile in any standard terminal command line interface:
```bash
# Clone the repository
git clone <your-repo-url>
cd <your-repo-folder>

# Compile using a modern GCC/Clang toolchain
g++ -std=c++11 main.cpp ServoController.cpp -o servo_sim

# Execute the binary
./servo_sim
```

 System Constraints & Constants
- MIN_ANGLE: 0° (Physical bottom bound)
(DELETE BEFORE SHIPING: make sure to set the angle to 0°)
- MAX_ANGLE: 180° (Physical top bound)
- MAX_SERVOS: 4 (Maximum allowable dynamic device nodes)

