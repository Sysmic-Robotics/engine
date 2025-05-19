add_library(logger STATIC
    logger.cpp
)

target_link_libraries(logger
    PRIVATE Qt6::Core
    PRIVATE utilities  # ✅ Now correctly linking to utilities for MotionCommand
    PRIVATE radio
    PRIVATE world
)

target_include_directories(motion PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}  
    ${CMAKE_CURRENT_SOURCE_DIR}/bangbangcontrol
    ${CMAKE_CURRENT_SOURCE_DIR}/pid
    ${CMAKE_SOURCE_DIR}/src/world
)
