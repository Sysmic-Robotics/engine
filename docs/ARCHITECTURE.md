# Engine Architecture Documentation

This document provides a top-down overview of the engine. Each chapter corresponds to a folder inside `src/` and explains its role within the system.

## Flow: Packet Reception to Robot Commands
1. External packets from vision and referee sources are captured by modules in `receivers`.
2. These packets are parsed and used to update the `world` state.
3. Each frame, `MainApp` triggers `LuaInterface` scripts that read the world and issue motion requests using functions in `motion`.
4. `Radio` packages these `RobotCommand` objects and transmits them to robots or the simulator.

## 1. `assets`
Contains application resources such as icons and Windows resource definitions (`engine_icon.ico`, `icon.rc`). These assets are compiled into the executable for branding and platform integration.

## 2. `consolereader`
Implements `ConsoleReader`, a `QThread` that monitors standard input for text commands. It understands commands like `run <script>`, `pause`, `resume`, `reload`, `record`, and `stoprecord`. Commands are forwarded to the `LuaInterface` to control script execution and emit recording signals consumed by `Logger`.

## 3. `luainterface`
Bridges Lua scripts with the C++ engine using `sol2`. It:
- Initializes and manages a Lua state.
- Registers numerous C++ functions for robot motion, ball interaction, telemetry retrieval, and simulator teleportation.
- Loads and executes user scripts, exposing `process()` callbacks executed every frame.
- Supports pausing, resuming, and error handling for scripts.

## 4. `logger`
Provides CSV logging of robot commands and states. When activated it writes a header followed by per-frame entries containing commanded velocities, actual poses, and ball state. Logging can be started and stopped via console commands.

## 5. `motion`
Motion planning and control algorithms:
- `environment`: Builds obstacle and field representations used for collision checks.
- `path_planner`: Computes obstacle-avoiding paths using a simple sub-goal strategy.
- `bangbangcontrol`: Generates velocity profiles along paths with acceleration limits.
- `pid`: Generic PID controller used for tracking and orientation control.
- `motion`: High-level motion functions such as `move_to`, `move_direct`, `motion_with_orientation`, and orientation-facing helpers.

## 6. `protobuf`
Auto-generated Protocol Buffer classes for communicating with external components such as grSim, SSL-Vision, and the GameController. They define message formats for robot commands, referee data, simulation control and vision packets.

## 7. `radio`
Handles transmission of commands to robots or the grSim simulator. `Radio` stores pending `RobotCommand` objects, serializes them, and sends via serial port or directly to grSim. It also offers teleportation utilities for robots and the ball. Supporting files handle packet serialization and grSim communication.

## 8. `receivers`
Modules that receive and process external data sources:
- `vision`: Listens to multicast UDP packets, decodes SSL-Vision frames, filters data with `Tracker`, and emits robot/ball updates.
- `tracker`: Maintains per-robot extended Kalman filters (`kalman`) to estimate velocities and orientation.
- `game_controller_ref` & `game_controller_team`: Clients for referee and team controllers, converting Protobuf messages to signals (`onRefCommand`) and handling registrations.
- `game_state`: Stores the last referee command received and provides it to Lua scripts.

## 9. `utilities`
Fundamental data structures describing the simulation state and robot commands:
- `RobotState` and `BallState` hold kinematic information.
- `MotionCommand`, `KickerCommand`, and `RobotCommand` represent outbound control messages.
- `StateChecker` offers small helpers for evaluating world conditions.

## 10. `websocketserver`
Provides a WebSocket endpoint on port 9001. It broadcasts world state as JSON and accepts messages for:
- Running or pausing Lua scripts.
- Direct joystick-style robot control, converting messages to `MotionCommand` and `KickerCommand` objects sent via `Radio`.

## 11. `world`
Central world model storing states for all robots and the ball. It receives updates from `Vision`, emits change signals, and serializes the complete scene to JSON for broadcasting. Each robot state tracks activity based on last update time.

## 12. `main.cpp`
`MainApp` orchestrates the system:
- Loads settings from `config.ini`.
- Initializes threads for vision, world update, referee client, and game state.
- Wires signal/slot connections among modules.
- Runs a 60 FPS loop that updates the world, executes Lua `process()` scripts, logs data, sends radio commands, and broadcasts state via WebSocket.

