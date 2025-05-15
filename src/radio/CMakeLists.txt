# Create a library target from radio.cpp
add_library(radio STATIC
    radio.cpp
    grsim.cpp
    packetserializer.cpp
)

target_link_libraries(radio
    #PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PUBLIC Qt6::SerialPort
    PUBLIC Qt6::Network
    PRIVATE utilities
    PUBLIC protobuf
    PRIVATE world
)

# Include the current directory for header files
target_include_directories(radio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
