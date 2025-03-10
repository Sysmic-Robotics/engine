# Create a library target from vision.cpp
add_library(skills
    move.cpp
    skill.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(skills
    Qt6::Core
    utilities
    motion
)

# Include the current directory for header files
target_include_directories(skills PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
