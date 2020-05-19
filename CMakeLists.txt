cmake_minimum_required(VERSION 3.15)
project(zktecotest C)

set(CMAKE_C_STANDARD 99)

if (WIN32)
    message("We run on win32! ")
    SET(OS_SPECIFIC win)
    set(MATH_LIB)

    if(CMAKE_CL_64)
        set(CMAKE_MSVC_ARCH amd64)
    else(CMAKE_CL_64)
        set(CMAKE_MSVC_ARCH x86)
    endif(CMAKE_CL_64)
endif(WIN32)

include_directories(${CMAKE_SOURCE_DIR}/zktekolib/include)
link_directories(${CMAKE_SOURCE_DIR}/zktekolib/x86lib)

add_executable(zktecotest main.c)
target_link_libraries(zktecotest libzkfp)