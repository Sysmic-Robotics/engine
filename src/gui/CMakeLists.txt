add_library(gui STATIC
    mainwindow.cpp
)

# Link Qt6 libraries to the Vision library
# Link to world and utilities (so it can use robotstate.hpp)
target_link_libraries(gui 
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Widgets
    PRIVATE world 
    PRIVATE utilities 
    )


target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
