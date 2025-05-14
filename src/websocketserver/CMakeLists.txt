# Create a library target from vision.cpp
add_library(websocketserver STATIC
    websocketserver.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(websocketserver
    Qt6::Core
    Qt6::WebSockets
    luainterface
    radio
    utilities
)

# Include the current directory for header files
target_include_directories(websocketserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
