add_subdirectory(receivers)
add_subdirectory(protobuf)
add_subdirectory(utilities)
add_subdirectory(world)
add_subdirectory(motion)
add_subdirectory(radio)
add_subdirectory(luainterface)
add_subdirectory(websocketserver)
add_subdirectory(consolereader)
add_subdirectory(logger)
# Specify sources in the src directory
qt_add_executable(engine
    main.cpp
)

if(WIN32)
  set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.rc")
  message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
  target_sources(engine PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()


# Link the Vision library and Qt6 libraries
target_link_libraries(engine
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
    PRIVATE consolereader
    PRIVATE logger
)
