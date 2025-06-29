# Encuentra Protobuf en el sistema


# Crea la librería de todos los Protobuf
add_library(protobuf
    ssl_vision_detection.pb.cc
    ssl_vision_geometry.pb.cc
    ssl_vision_wrapper.pb.cc

    grSim_Packet.pb.cc
    grSim_Replacement.pb.cc
    grSim_Commands.pb.cc
    ssl_gc_common.pb.cc
    ssl_game_controller_team.pb.cc
    ssl_game_controller_geometry.pb.cc
    ssl_referee_game_event.pb.cc
    ssl_game_controller_auto_ref.pb.cc
    ssl_game_event_2019.pb.cc   
    ssl_referee.pb.cc 
    ssl_game_controller_common.pb.cc  
)

# Incluye los headers generados automáticamente
target_include_directories(protobuf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}           # Para incluir los .pb.h
    ${Protobuf_INCLUDE_DIRS}              # Para que encuentre port_def.inc y demás headers internos
)

# Linkea con la librería de Protobuf correctamente
target_link_libraries(protobuf PUBLIC protobuf::libprotobuf)

# NOTA:
# Si quieres automatizar la generación desde los archivos .proto ubicados en `protos/`,
# puedes agregar un script o función para compilar los .proto con protoc antes de esta sección.
