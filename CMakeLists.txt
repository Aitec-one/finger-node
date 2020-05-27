cmake_minimum_required(VERSION 3.9)

IF (CMAKE_JS_VERSION)
    project(finger)
    set(CMAKE_CXX_STANDARD 11)

    message("Cmake js lib is: ${CMAKE_JS_LIB}")
    add_definitions(-DCMAKE_JS)

    set(NAPI_INCLUDES_DIR node_modules/node-addon-api ../node-addon-api)

    IF (WIN32)
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/include)
        set(ZKTEKO_LIB_DIR ${CMAKE_SOURCE_DIR}/zktekolib/win/x64lib/)
        set(ZKTEKO_LIB ${ZKTEKO_LIB_DIR}/libzkfp.lib)
    ELSE()
        set(ZKTECO_INCLUDES_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/include)
        set(ZKTEKO_LIB_DIR ${CMAKE_SOURCE_DIR}/zktekolib/unix/lib-x64/)
        set(ZKTEKO_LIB ${ZKTEKO_LIB_DIR}/libzkfp.so)
    ENDIF()

    file(GLOB SOURCE_FILES
            ./napi_addon/Addon.cpp
            ./napi_addon/FingerVerifyAsyncWorker.cpp
            ./napi_addon/FingerEnrollAsyncWorker.cpp
            FingerScanner.cpp
            base64.cpp)

    IF (CMAKE_COMPILER_IS_GNUCC)
        # needed for gcc 4.6+
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
    ENDIF()

    include_directories("${CMAKE_SOURCE_DIR}/includes")
    include_directories("${CMAKE_SOURCE_DIR}/napi_addon/includes")
    include_directories("${CMAKE_JS_INC}")

    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

    target_include_directories(${PROJECT_NAME} PRIVATE ${ZKTECO_INCLUDES_DIR})
    # Include N-API wrappers
    target_include_directories(${PROJECT_NAME} PRIVATE ${NAPI_INCLUDES_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

    IF (WIN32)
        target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${ZKTEKO_LIB})
    ELSE()
        message("Importing shared zktecolib as fingerlib")

#        add_library(fingerlib SHARED IMPORTED)
#        set_target_properties(fingerlib PROPERTIES IMPORTED_LOCATION ${ZKTEKO_LIB})
#        target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} fingerlib)
#        link_directories(${ZKTEKO_LIB_DIR})
        target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} -lpthread -ldl)
        target_link_libraries(${PROJECT_NAME} -L${ZKTEKO_LIB_DIR})
    ENDIF()

ELSE()
    project(zktecotest)
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

    include_directories(${CMAKE_SOURCE_DIR}/includes)
    include_directories(${ZKTECO_INCLUDES_DIR})

    add_library(finger FingerScanner.cpp base64.cpp)

    IF (WIN32)
        target_link_libraries(${PROJECT_NAME} ${ZKTEKO_LIB} finger)
    ELSE ()
        target_link_libraries(finger -lpthread -ldl -L${ZKTEKO_LIB_DIR})
#
    ENDIF()

    add_subdirectory(finger_test)

ENDIF()