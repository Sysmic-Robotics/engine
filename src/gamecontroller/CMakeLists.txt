add_library(gamecontroller STATIC
    gamecontroller.cpp
)

target_link_libraries(gamecontroller
    PRIVATE Qt6::Core
    PRIVATE protobuf 
)
