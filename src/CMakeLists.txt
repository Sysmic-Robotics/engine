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


if (WIN32)
  # Find windeployqt (PATH, QT_ROOT, or deduce from Qt6_DIR)
  find_program(WINDEPLOYQT_EXECUTABLE
    NAMES windeployqt windeployqt.exe
    HINTS
      "$ENV{QT_ROOT}/bin"
      "${Qt6_DIR}/../../../bin"   # from .../lib/cmake/Qt6 -> .../mingw_64/bin
  )

  # Hard fail at configure time if not found (prevents "" does not exist at build time)
  if (NOT WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR
      "windeployqt not found.\n"
      "Tried: $ENV{QT_ROOT}/bin and ${Qt6_DIR}/../../../bin\n"
      "Set QT_ROOT (e.g. C:/Qt/6.9.0/mingw_64), or add Qt's bin to PATH,\n"
      "or pass -DWINDEPLOYQT_EXECUTABLE=C:/Qt/6.9.0/mingw_64/bin/windeployqt.exe")
  endif()

  # (Optional) echo so you can see the resolved paths
  message(STATUS "Qt6_DIR: ${Qt6_DIR}")
  message(STATUS "windeployqt: ${WINDEPLOYQT_EXECUTABLE}")

  # Deploy to the build dir (directory style)
  set(_app_dir "$<TARGET_FILE_DIR:engine>")

  add_custom_command(TARGET engine POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E echo "windeployqt dir: ${_app_dir}"
    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
      $<$<CONFIG:Debug>:--debug>
      $<$<CONFIG:RelWithDebInfo,Release,MinSizeRel>:--release>
      --compiler-runtime
      "${_app_dir}"
    COMMENT "Running windeployqt for directory ${_app_dir}"
    VERBATIM
  )
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
    Eigen3::Eigen
)
