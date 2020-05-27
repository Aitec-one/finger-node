cmake_minimum_required(VERSION 3.15)
project(zktecotest)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
    set(NAPI_INCLUDES_DIR node_modules/node-addon-api ../node-addon-api)
    set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/include)
    add_definitions(-DCMAKE_JS)

    include_directories("${CMAKE_JS_INC}")
    include_directories("${ZKTECO_INCLUDES_DIR}")

    file(GLOB SOURCE_FILES Addon.cpp DataProcessingAsyncWorker.cpp DataProcessingAsyncWorker.h FingerScanner.cpp FingerScanner.h base64.cpp base64.h)

    if (CMAKE_COMPILER_IS_GNUCC)
        # needed for gcc 4.6+
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
    endif ()

    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
    target_include_directories(${PROJECT_NAME} PRIVATE ${ZKTECO_INCLUDES_DIR})

    message("Cmake js lib is: ${CMAKE_JS_LIB}")

    # Include N-API wrappers
    target_include_directories(${PROJECT_NAME} PRIVATE ${NAPI_INCLUDES_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

#    link_directories(${CMAKE_SOURCE_DIR}/zktekolib/x64lib)
    target_link_libraries(
            ${PROJECT_NAME}
            ${CMAKE_JS_LIB}
            ${CMAKE_SOURCE_DIR}/zktekolib/x64lib/libzkfp.lib
    )

else ()
    include_directories(${CMAKE_SOURCE_DIR}/zktekolib/include)
    link_directories(${CMAKE_SOURCE_DIR}/zktekolib/x64lib)

    add_executable(${PROJECT_NAME} finger_test.cpp FingerScanner.cpp FingerScanner.h base64.cpp base64.h)
    target_link_libraries(${PROJECT_NAME} libzkfp)
endif ()