# Create a library target from world.cpp
add_library(world STATIC
    world.cpp
)

target_link_libraries(world
    PRIVATE Qt6::Core
    PRIVATE utilities  # Link the utilities library
)

# Include the current directory for header files
target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
