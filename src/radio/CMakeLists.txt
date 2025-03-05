# Create a library target from radio.cpp
add_library(radio STATIC
    radio.cpp
    grsim.cpp
)

target_link_libraries(radio
    #PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE Qt6::Network
    PRIVATE motion
    PRIVATE protobuf
)

# Include the current directory for header files
target_include_directories(radio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
