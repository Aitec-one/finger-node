cmake_minimum_required(VERSION 3.9)

project(finger_test)
set(CMAKE_CXX_STANDARD 11)

include_directories(${ZKTECO_INCLUDES_DIR})
include_directories(${FINGER_INCLUDES_DIR})

add_executable(${PROJECT_NAME} finger_test.cpp)

IF (WIN32)
    target_link_libraries(finger_test ${ZKTEKO_LIB})
ELSE ()
    target_link_libraries(finger_test finger)
ENDIF()