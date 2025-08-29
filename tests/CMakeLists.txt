add_executable(robotstate_test robotstate_test.cpp)
target_include_directories(robotstate_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src/utilities
)
target_link_libraries(robotstate_test Qt6::Test utilities)
add_test(NAME robotstate_test COMMAND robotstate_test)

add_executable(world_test world_test.cpp)
target_include_directories(world_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src/utilities
    ${CMAKE_SOURCE_DIR}/src/world
)
target_link_libraries(world_test Qt6::Test utilities world)
add_test(NAME world_test COMMAND world_test)
