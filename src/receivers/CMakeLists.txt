# Create a library target from vision.cpp
add_library(receivers
    vision.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(receivers
    Qt6::Core
    Qt6::Network
    protobuf
    utilities
)

# Include the current directory for header files
target_include_directories(receivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
