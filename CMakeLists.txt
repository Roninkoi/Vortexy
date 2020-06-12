cmake_minimum_required(VERSION 3.2)
project(Vortexy)

option(SIMONLY "Exclude renderer" OFF)
option(BUILD_SHARED_LIBS OFF)

if (SIMONLY)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -static") # -O3
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11") # -O3
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message("RENDERER: " NOT ${SIMONLY})

#find_library(LM NAMES libm.a)
#message("math: " ${LM})

#find_library(LC NAMES libc.a)
#message("libc: " ${LC})

set(CMAKE_SKIP_BUILD_PATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

set(EXCLUDE_DIR ".*src/render/*")
file(
        GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

if (SIMONLY)
    list(FILTER SOURCES EXCLUDE REGEX ${EXCLUDE_DIR})
endif ()
message(${SOURCES})

add_executable(
        Vortexy ${SOURCE_FILES} ${SOURCES}
)

set_target_properties(Vortexy PROPERTIES LINK_FLAGS -Wl,-rpath=$ORIGIN/lib,-z,origin,-allow-multiple-definition#,-whole-archive,-as-needed
        )

if (SIMONLY)
    target_link_libraries(Vortexy m)
else ()
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)

	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include)
	target_link_libraries(Vortexy m "${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/lib/libGLEW.a" GL glfw)
    # lapacke

	#target_link_libraries(Vortexy m GLEW GL glfw)
endif ()
