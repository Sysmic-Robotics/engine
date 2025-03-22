
# CondorSSL
![logo2](https://github.com/Sysmic-Robotics/CondorSSL/blob/main/docs/logo.jpg)

This project consists of two parts. The first part is the engine, which consists of the main program used to control the robots. On the other hand, the second part is a graphical interface intended to accelerate the development (debugging and testing) of the engine.

## 1 Requirements

For the development of this project the following requirements are needed:

### 1.1 Linux SO

This project uses Linux for development, preferably Pop!_OS or Ubuntu.

Pop!_OS installation guide:
https://www.youtube.com/watch?v=eol5O1qfWNs

### 1.2 Grsim

For this project we use grsim simulator, and it can be installed from the following link:
https://github.com/RoboCup-SSL/grSim

### 1.3 VSCode

For the code development we strongly recommend using VSCode code editor. The settings to run the engine program and GUI program are located in the .vscode folder.

### 1.4 Lua (for embedding)

The project also requires the Lua development libraries. Install them with the following command:

```bash
sudo apt install liblua5.3-dev
```

This is necessary for embedding Lua scripting support within the engine.

### 1.5 CMake

First, install **CMake in your system**. The project requires **CMake version 3.16 or higher**

To ensure you have a compatible version, it's recommended to install it manually:

```bash
sudo apt-get install build-essential libssl-dev
wget https://github.com/Kitware/CMake/releases/download/v3.20.5/cmake-3.20.5.tar.gz
tar -zxvf cmake-3.20.5.tar.gz
cd cmake-3.20.5
./bootstrap
make
sudo make install
cmake --version
```

> You may replace `3.20.5` with any version equal to or higher than `3.16`.
> The latest versions are available at: https://github.com/Kitware/CMake/releases

These commands work for both **Pop!_OS** and **Ubuntu**.

### 1.6 CMake Tools extension for VSCode

After installing CMake system-wide, open **VSCode** and install the extension **"CMake Tools"** from the extensions panel.

#### In case the CMake icon does not appear:
- Restart VSCode.
- If it still doesn't show up, press `Ctrl + Shift + P`, search for `CMake: Configure`, and run it.
- It will ask you to select a compiler. Choose the GCC version available (e.g. `/usr/bin/gcc`).

After that, the CMake icon should appear in the left sidebar.

### 1.7 Qt6

Download and install Qt6 using the official installer: https://www.qt.io/download
Use the GUI installer, not the terminal. Simply install it; **you do not need to manually open Qt Creator after installation**.

### 1.8 Running the project with CMake Tools

Once all dependencies are installed and the project is opened in VSCode, follow these steps to run the project:

1. **Open the grSim application**, which you installed earlier in step 1.2.  
2. Click on the **CMake Tools icon** in the left sidebar.  
3. Press **"Configure"** to generate the initial build configuration.  
4. After configuration finishes, press **"Build"** to compile the code.  
5. Finally, press **"Debug"** to launch the application and communicate with grSim.

> After building the project once, for future runs you only need to open **grSim** and then press **"Debug"** in VSCode. There's no need to reconfigure or rebuild unless you modify the code.

#### Possible error during configuration

In some systems, you might encounter the following CMake configuration error:

```
Could not find a package configuration file provided by "Qt6" with any of the following names:
Qt6Config.cmake
qt6-config.cmake
```

If this happens, follow these steps:

1. Locate the correct path to your Qt6 installation. You can do this by running the following command in your terminal:

```bash
find ~ -name "Qt6Config.cmake" 2>/dev/null
```

2. Copy the path that looks like:
```
/home/youruser/Qt/6.x.x/gcc_64/lib/cmake/Qt6
```

3. In VSCode, press `Ctrl + Shift + P`, then search for `CMake: Edit Cache (UI)`.  
4. Look for the field named `Qt6_DIR` and paste the path you copied there.  
5. Press `Configure` again.

> This step is only required if the automatic detection fails. Most users will not need to do this.

---

