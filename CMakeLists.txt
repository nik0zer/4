cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

SET(MYCOMPILE_FLAGS "-pthread")

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${MYCOMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")

file(GLOB SRC "src/*")

add_executable(task_4 ${SRC})

target_include_directories(task_4 PRIVATE "include")