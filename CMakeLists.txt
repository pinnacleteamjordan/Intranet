CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Intranet)

SET(Intranet_VERSION_MAJOR 0)
SET(Intranet_VERSION_MINOR 1)

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/IntranetConfig.h.in"
    "${PROJECT_SOURCE_DIR}/IntranetConfig.h"
)

FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(PostgreSQL REQUIRED)
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
)

ADD_EXECUTABLE(intranet
    main.cpp
)
TARGET_LINK_LIBRARIES(intranet
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
)
