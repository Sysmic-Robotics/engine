
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

   * MinGW (https://winlibs.com/)
   * Qt6 (with SerialPort and WebSockets)
   * CMake  3.31 (https://cmake.org/files/) 
   * VSCode with **CMake Tools** extension
   * grSim simulator ([grSim GitHub](https://github.com/RoboCup-SSL/grSim))

2. Configure :


   * Add MinGW location to CMake Tools using Configure
   * Add Cmake Path to VSCode Settings

3. Run :
   * Build using CMake Tools
   * Run code by launching the debugger

4. Opcional use `windeployqt` to bundle dependencies:

```cmd
windeployqt C:\CondorSSL\build\engine.exe
```

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




## 👥 Contributors

<p align="center">
  <a href="https://github.com/Sysmic-Robotics/CondorSSL/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=Sysmic-Robotics/CondorSSL" />
  </a>
</p>



## 📄 License

This project is licensed under the terms of the [MIT License](https://github.com/Sysmic-Robotics/CondorSSL/blob/main/LICENSE).


