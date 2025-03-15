# Create a library target from vision.cpp
add_library(pathplannig STATIC
    pathplanning.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(pathplannig
    Qt6::Core
    Qt6::Gui   
)

# Include the current directory for header files
target_include_directories(pathplannig PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
