# Create a library target
add_library(motion STATIC
    motion.cpp
    motioncommand.cpp
)

# Link Qt6 modules, including GUI for QVector2D
target_link_libraries(motion
    PUBLIC Qt6::Gui  # Ensure Qt6Gui is linked for QVector2D
    PRIVATE Qt6::Core
    PRIVATE protobuf
    PRIVATE utilities
)

# Include the current directory for header files
target_include_directories(motion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
