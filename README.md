
<p align="center">
  <img src="https://i.imgur.com/vU1zy4V.png" alt="CondorSSL Logo" width="256"/>
</p>

<h1 align="center">Engine</h1>

<p align="center">
  <a href="https://github.com/Sysmic-Robotics/CondorSSL/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/Sysmic-Robotics/CondorSSL" alt="License">
  </a>
  <a href="https://github.com/Sysmic-Robotics/CondorSSL">
    <img src="https://img.shields.io/github/v/release/Sysmic-Robotics/CondorSSL" alt="Version">
  </a>
</p>

<p align="center">
  This is a desktop engine and development interface built for robot control and debugging, using C++, Qt6, and Lua scripting. It is part of the Sysmic Robotics control system.
</p>



## 🧩 Table of Contents

- [Installation](#installation)
- [Development Usage](#development-usage)
- [Production Deployment](#production-deployment)
- [Contributors](#contributors)
- [License](#license)

---

## 📥 Installation

```bash
git clone https://github.com/Sysmic-Robotics/CondorSSL.git
cd CondorSSL
````

Follow platform-specific instructions in the [setup guide](#installation) below.



## 🛠️ Development Usage

### Windows

1. Install:

   * MinGW (https://winlibs.com/) [Download Linl](https://github.com/brechtsanders/winlibs_mingw/releases/download/15.1.0posix-13.0.0-msvcrt-r4/winlibs-x86_64-posix-seh-gcc-15.1.0-mingw-w64msvcrt-13.0.0-r4.zip)
   * Qt6 (with SerialPort and WebSockets) [Download Link](https://www.qt.io/download-qt-installer-oss)
   * CMake  3.31 ([Download Link](https://github.com/Kitware/CMake/releases/download/v3.31.8/cmake-3.31.8-windows-x86_64.msi)) 
   * VSCode with **CMake Tools** extension
   * grSim simulator ([grSim GitHub](https://github.com/RoboCup-SSL/grSim))

2. Configure :
   * Add MinGW location to CMake Tools using Configure
   * Add Cmake Path to VSCode Settings
   * Create a build folder inside the project
3. Use the windeployqt program from qt6 installation directory to copy the .dll qt6 files into the build folder
   Example:
    ```cmd
      windeployqt C:\CondorSSL\build\engine.exe
      ```
4. Run :
   * Build using CMake Tools
   * Run code by launching the debugger

---

### Linux

1. Install:

   * Qt6 (gcc\_64 with SerialPort and WebSockets)
   * Lua (e.g. `sudo apt install liblua5.3-dev`)
   * grSim simulator ([grSim GitHub](https://github.com/RoboCup-SSL/grSim))
   

2. Build and Run in VSCode:

   * Open Command Palette → CMake: Configure
   * Then: Build → Debug

> If Qt6 is not found, manually set the `Qt6_DIR` using `CMake: Edit Cache (UI)`.

## 💬 Commands

| Command         | Description                                      |
|----------------|--------------------------------------------------|
| `run <path>`   | Run a Lua script from the specified path         |
| `pause`        | Pause the currently running script               |
| `reload`       | Reload the last successfully run script          |
| `exit`         | Exit the console reader thread (does not quit app) |

## Documentation

A brief documentation of the program architecture can be found in docs/architecture. Also there is additional information about Lua functions in the Wiki section of this github repository.



## 👥 Contributors

<p align="center">
  <a href="https://github.com/Sysmic-Robotics/engine/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=Sysmic-Robotics/engine" />
  </a>
</p>





## 📄 License

This project is licensed under the terms of the [MIT License](https://github.com/Sysmic-Robotics/CondorSSL/blob/main/LICENSE).


