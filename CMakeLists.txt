cmake_minimum_required(VERSION 3.2)
project(Vortexy)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11") # -O3

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#include_directories(${GLFW_INCLUDE_DIRS})

#pkg_search_module(GLEW REQUIRED glew)
#include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
if(NOT OPENCL_FOUND)
  message(ERROR "OPENCL not found!")
endif(NOT OPENCL_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
  message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

set(CMAKE_SKIP_BUILD_PATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

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

set_target_properties(Vortexy PROPERTIES LINK_FLAGS -Wl,-rpath=$ORIGIN/lib,-z,origin,-allow-multiple-definition)

#target_link_libraries(Vortexy ${OpenCL_LIBRARY} m GLEW GL ${GLFW_STATIC_LIBRARIES})

target_link_libraries(Vortexy #${OpenCL_LIBRARY}
  m GLEW GL glfw)
#target_link_libraries(Vortexy /usr/lib64/libOpenCL.so.1)

