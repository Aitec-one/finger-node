cmake_minimum_required(VERSION 3.15)
project(zktecotest)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/zktekolib/include)
link_directories(${CMAKE_SOURCE_DIR}/zktekolib/x86lib)

add_executable(zktecotest main.cpp FingerScanner.cpp FingerScanner.h)
target_link_libraries(zktecotest libzkfp)