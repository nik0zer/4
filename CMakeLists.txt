cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

SET(MYCOMPILE_FLAGS "-pthread")

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${MYCOMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")

project("Kaspersky_tasks_c++/4")

enable_testing()
add_subdirectory(googletest)

file(GLOB SRC "src/*")
file(GLOB TEST_SRC "test_src/*")

add_executable(task_4 main.cpp ${SRC})
add_executable(test_task_4 ${TEST_SRC} ${SRC})

target_include_directories(task_4 PUBLIC "include")
target_include_directories(test_task_4 PUBLIC "include")
target_link_libraries(task_4 PUBLIC gtest_main gmock_main)
target_link_libraries(test_task_4 PUBLIC gtest_main gmock_main)