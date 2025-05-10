add_subdirectory(bangbangcontrol)  # ✅ Ensure bangbangcontrol is added
add_subdirectory(pid)  # ✅ Ensure bangbangcontrol is added

add_library(motion STATIC
    motion.cpp
    path_planner.cpp  # ✅ Agregado el nuevo archivo de planificación de trayectoria
)

target_link_libraries(motion
    PUBLIC Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE protobuf
    PRIVATE utilities  # ✅ Now correctly linking to utilities for MotionCommand
    PRIVATE bangbangcontrol  # ✅ Still linking to bangbangcontrol
    PRIVATE pid
    PRIVATE world
)

target_include_directories(motion PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}  
    ${CMAKE_CURRENT_SOURCE_DIR}/bangbangcontrol
    ${CMAKE_CURRENT_SOURCE_DIR}/pid
    ${CMAKE_SOURCE_DIR}/src/world
)
