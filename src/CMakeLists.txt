add_subdirectory(receivers)
add_subdirectory(protobuf)
add_subdirectory(utilities)
add_subdirectory(world)
add_subdirectory(motion)
add_subdirectory(radio)
add_subdirectory(luainterface)
add_subdirectory(websocketserver)

# Specify sources in the src directory
qt_add_executable(SysmicSoftware
    main.cpp
)

# Link the Vision library and Qt6 libraries
target_link_libraries(SysmicSoftware
    PRIVATE Qt6::Widgets  # ✅ Ensure Qt6::Widgets is linked
    PRIVATE Qt6::Gui  # ✅ Ensure Qt6Gui is linked
    PRIVATE receivers
    PRIVATE utilities
    PRIVATE world
    PRIVATE protobuf
    PRIVATE motion
    PRIVATE radio
    PRIVATE luainterface
    PRIVATE lua_static
    PRIVATE sol2::sol2
    PRIVATE websocketserver
)
