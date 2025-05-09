# Create a library target from consolereader.cpp
add_library(consolereader STATIC
    consolereader.cpp
)

# Link Qt6 libraries to the consolereader library
target_link_libraries(consolereader
    Qt6::Core
    luainterface
)

# Include the current directory for header files
target_include_directories(consolereader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
