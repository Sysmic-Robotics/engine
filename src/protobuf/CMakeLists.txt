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

    ssl_gc_referee_message.pb.cc
    ssl_gc_game_event.pb.cc
    ssl_gc_geometry.pb.cc
)

# Include the Protobuf generated headers
target_include_directories(protobuf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Protobuf to the library
#find_package(Protobuf REQUIRED)
target_link_libraries(protobuf PUBLIC protobuf::libprotobuf)

# Ensure the external Protobuf build/install completes before this target
add_dependencies(protobuf project_protobuf project_protobuf-install)
