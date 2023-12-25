cmake_minimum_required(VERSION 3.22.1)

project(iqtcplib VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(LibSndFile REQUIRED)

include_directories(SYSTEM ${LIBSNDFILE_INCLUDE_DIR})
set(LIBS ${LIBSNDFILE_LIBRARY})

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PRIVATE
            src/TcpInput.cpp
            src/TcpOutput.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            sndfile
)
