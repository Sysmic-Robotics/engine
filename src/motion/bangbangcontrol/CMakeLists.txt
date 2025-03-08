add_library(bangbangcontrol STATIC
    bangbangcontrol.cpp
    trajectory2d.cpp
    trajectory1d.cpp
)

# Link required libraries
target_link_libraries(bangbangcontrol
    PUBLIC Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE utilities  # ✅ Now correctly linking to utilities for MotionCommand
)

# Include directories
target_include_directories(bangbangcontrol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
