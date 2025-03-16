add_subdirectory(receivers)
add_subdirectory(protobuf)
add_subdirectory(utilities)
add_subdirectory(world)
add_subdirectory(gui)
add_subdirectory(motion)
add_subdirectory(radio)
add_subdirectory(stp)
add_subdirectory(luainterface)

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
    PRIVATE gui
    PRIVATE motion
    PRIVATE radio
    PRIVATE skills
    PRIVATE luainterface
)
