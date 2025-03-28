add_library(luainterface STATIC
    luainterface.cpp
)

target_link_libraries(luainterface
    PRIVATE lua_static
    PRIVATE sol2::sol2
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE radio
    PRIVATE utilities
    PRIVATE motion
    PRIVATE world
)

target_include_directories(luainterface 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  
)
