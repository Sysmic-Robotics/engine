# Create a library target from vision.cpp
add_library(utilities STATIC
    robotstate.cpp
    motioncommand.cpp
    statechecker.cpp
    ballstate.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(utilities
    Qt6::Gui
)

# Include the current directory for header files
target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
