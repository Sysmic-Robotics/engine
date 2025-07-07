# Create a library target from vision.cpp and KalmanFilter.cpp
add_library(receivers
    vision.cpp
    kalman/ekf.cpp
    tracker.cpp
    game_controller_ref.cpp
    game_state.cpp
)

# Link Qt6 libraries to the Vision library
target_link_libraries(receivers
    Qt6::Core
    Qt6::Network
    protobuf
    utilities
    Eigen3::Eigen
)

# Include headers from current dir and Kalman subdirectory
target_include_directories(receivers PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/kalman
)
