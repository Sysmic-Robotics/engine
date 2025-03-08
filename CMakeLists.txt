cmake_minimum_required(VERSION 3.16)

project(SysmicSoftware VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")



find_package(Qt6 REQUIRED COMPONENTS Core Network Gui Widgets)
# The qt_standard_project_setup command sets project-wide defaults for a typical Qt application.
qt_standard_project_setup()

# Add the src directory
add_subdirectory(src)