# Create a library for the Proto files
add_library(protobuf
    ssl_vision_detection.pb.cc
    ssl_vision_geometry.pb.cc
    ssl_vision_wrapper.pb.cc

    grSim_Packet.pb.cc
    grSim_Replacement.pb.cc
    grSim_Commands.pb.cc

    ssl_gc_rcon_team.pb.cc
    ssl_gc_rcon.pb.cc
    ssl_gc_common.pb.cc
)

# Include the Protobuf generated headers
target_include_directories(protobuf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Protobuf to the library
#find_package(Protobuf REQUIRED)
target_link_libraries(protobuf PUBLIC protobuf::libprotobuf)