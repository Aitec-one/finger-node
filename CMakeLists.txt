cmake_minimum_required(VERSION 3.15)
project(zktecotest C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR}/zktekolib/include)
link_directories(${CMAKE_SOURCE_DIR}/zktekolib/x86lib)

add_executable(zktecotest main.c)
target_link_libraries(zktecotest libzkfp)