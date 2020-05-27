cmake_minimum_required(VERSION 3.9)

project(finger_test)
set(CMAKE_CXX_STANDARD 11)


IF (WIN32)
    set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/include)
    set(ZKTEKO_LIB_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/x64lib/)
    set(ZKTEKO_LIB ${ZKTEKO_LIB_DIR}/libzkfp.lib)
ELSE()
    set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/include)
    set(ZKTEKO_LIB_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/lib-x64/)
    set(ZKTEKO_LIB ${ZKTEKO_LIB_DIR}/libzkfp.so)
ENDIF()

include_directories(${ZKTECO_INCLUDES_DIR})
include_directories(${PROJECT_SOURCE_DIR}/includes)

add_executable(${PROJECT_NAME} finger_test.cpp)

IF (WIN32)
    target_link_libraries(finger ${ZKTEKO_LIB})
ELSE ()
    target_link_libraries(finger -lpthread -ldl -L${ZKTEKO_LIB_DIR})
ENDIF()