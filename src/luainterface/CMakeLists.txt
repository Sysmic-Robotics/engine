add_library(luainterface STATIC
    luainterface.cpp
)

target_link_libraries(luainterface
    PRIVATE ${LUA_LIBRARIES}
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
)

target_include_directories(luainterface 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  
)
