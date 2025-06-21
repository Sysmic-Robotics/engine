add_library(pi_vel STATIC
    pi_vel.cpp
)

# Link required libraries
target_link_libraries(pid
    PUBLIC Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE utilities  # ✅ Now correctly linking to utilities for MotionCommand
)

# Include directories
target_include_directories(pi_vel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
