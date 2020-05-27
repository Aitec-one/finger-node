cmake_minimum_required(VERSION 3.9)

IF (CMAKE_JS_VERSION)
    project(finger)
    set(CMAKE_CXX_STANDARD 11)

    set(NAPI_INCLUDES_DIR node_modules/node-addon-api ../node-addon-api)

    IF (WIN32)
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/include)
        set(ZKTEKO_LIB ${CMAKE_SOURCE_DIR}/zktekolib/win/x64lib/libzkfp.lib)
    ELSE()
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/include)
        set(ZKTEKO_LIB ${CMAKE_SOURCE_DIR}/zktekolib/unix/lib-x64/libzkfp.so)
    ENDIF()

    add_definitions(-DCMAKE_JS)

    include_directories("${CMAKE_JS_INC}")
    include_directories("${ZKTECO_INCLUDES_DIR}")

    file(GLOB SOURCE_FILES Addon.cpp FingerVerifyAsyncWorker.cpp FingerVerifyAsyncWorker.h FingerEnrollAsyncWorker.cpp FingerEnrollAsyncWorker.h FingerScanner.cpp FingerScanner.h base64.cpp base64.h)

    IF (CMAKE_COMPILER_IS_GNUCC)
        # needed for gcc 4.6+
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
    ENDIF()

    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

    target_include_directories(${PROJECT_NAME} PRIVATE ${ZKTECO_INCLUDES_DIR})

    message("Cmake js lib is: ${CMAKE_JS_LIB}")

    # Include N-API wrappers
    target_include_directories(${PROJECT_NAME} PRIVATE ${NAPI_INCLUDES_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

    IF (WIN32)
        target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${ZKTEKO_LIB})
    ELSE()
        message("Importing shared zktecolib as fingerlib")

        add_library(fingerlib SHARED IMPORTED)
        set_target_properties(fingerlib PROPERTIES IMPORTED_LOCATION ${ZKTEKO_LIB})
        target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} fingerlib)
    ENDIF()

ELSE()
    project(zktecotest)
    set(CMAKE_CXX_STANDARD 11)

    set(NAPI_INCLUDES_DIR node_modules/node-addon-api ../node-addon-api)
    IF (WIN32)
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/include)
        set(ZKTEKO_LIB ${CMAKE_SOURCE_DIR}/zktekolib/win/x64lib/libzkfp.lib)
    ELSE()
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/include)
        set(ZKTEKO_LIB ${CMAKE_SOURCE_DIR}/zktekolib/unix/lib-x64/libzkfp.so)
    ENDIF()

    include_directories(${ZKTECO_INCLUDES_DIR})
    include_directories(${NAPI_INCLUDES_DIR})

    add_executable(${PROJECT_NAME} finger_test.cpp Addon.cpp FingerVerifyAsyncWorker.cpp FingerVerifyAsyncWorker.h FingerEnrollAsyncWorker.cpp FingerEnrollAsyncWorker.h FingerScanner.cpp FingerScanner.h base64.cpp base64.h)
    target_link_libraries(${PROJECT_NAME} ${ZKTEKO_LIB})
ENDIF()