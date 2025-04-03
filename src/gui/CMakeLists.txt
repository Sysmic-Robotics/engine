# Create a static library for the GUI
add_library(gui STATIC
    mainwindow.cpp
    robotitem.cpp
    targetmarker.cpp  # ✅ Fixed filename
    ballitem.cpp
    robotcontrolpanel.cpp
    scriptcontrolpanel.cpp
)

# Link Qt6 libraries to the GUI library
target_link_libraries(gui 
    PRIVATE Qt6::Core  # ✅ Needed for QVector2D and QtCore functionality
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Widgets  # ✅ Changed to PRIVATE
    PRIVATE world 
    PRIVATE utilities 
)

# Include the current directory for header files
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
