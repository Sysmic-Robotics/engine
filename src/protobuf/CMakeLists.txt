# Create a library for the Proto files
add_library(protobuf
    messages_robocup_ssl_detection.pb.cc
    messages_robocup_ssl_geometry.pb.cc
    messages_robocup_ssl_wrapper.pb.cc

    grSim_Packet.pb.cc
    grSim_Replacement.pb.cc
    grSim_Commands.pb.cc
)

# Include the Protobuf generated headers
target_include_directories(protobuf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Protobuf to the library
find_package(Protobuf REQUIRED)
target_link_libraries(protobuf PUBLIC protobuf::libprotobuf)