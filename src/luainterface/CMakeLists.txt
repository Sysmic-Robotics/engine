add_library(luainterface STATIC
    luainterface.cpp
    luabindings.cpp
)

target_link_libraries(luainterface
    PUBLIC lua_static
    PUBLIC sol2::sol2
    PUBLIC Qt6::Gui
    PUBLIC Qt6::Core
    PUBLIC radio
    PUBLIC utilities
    PUBLIC motion
    PUBLIC world
    PUBLIC receivers
)

target_include_directories(luainterface 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  
)
