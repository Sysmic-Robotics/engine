add_subdirectory(receivers)
add_subdirectory(protobuf)
add_subdirectory(utilities)
add_subdirectory(world)
add_subdirectory(gui)
add_subdirectory(grsim)
# Specify sources in the src directory
qt_add_executable(SysmicSoftware
    main.cpp
)


# Link the Vision library and Qt6 libraries
target_link_libraries(SysmicSoftware
    PRIVATE receivers
    PRIVATE utilities
    PRIVATE world
    PRIVATE protobuf
    PRIVATE gui
    PRIVATE grsim
    PRIVATE Qt6::Widgets  # ✅ Ensure Qt6::Widgets is linked
)

