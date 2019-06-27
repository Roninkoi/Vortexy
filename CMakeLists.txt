cmake_minimum_required(VERSION 3.2)
project(Vortexy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11") # -O3

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

file(
        GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(
        Vortexy ${SOURCE_FILES} ${SOURCES}
)

target_link_libraries(Vortexy OpenCL)
#target_link_libraries(Vortexy /usr/lib64/libOpenCL.so.1)

target_link_libraries(Vortexy "-Wl,--allow-multiple-definition")
