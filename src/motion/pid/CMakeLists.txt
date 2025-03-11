add_library(pid STATIC
    pid.cpp
)

# Link required libraries
target_link_libraries(pid
    PUBLIC Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE utilities  # ✅ Now correctly linking to utilities for MotionCommand
)

# Include directories
target_include_directories(pid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
