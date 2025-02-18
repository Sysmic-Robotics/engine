# Create a library target from world.cpp
add_library(grsim STATIC
    grsim.cpp
)

target_link_libraries(grsim
    PRIVATE protobuf
    PRIVATE Qt6::Core
    PRIVATE Qt6::Network
)

# Include the current directory for header files
target_include_directories(grsim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
