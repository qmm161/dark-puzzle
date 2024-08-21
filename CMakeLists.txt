cmake_minimum_required (VERSION 3.0)

project (puzzle)

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

message("build puzzle...")
set(CMAKE_C_COMPILER "/Library/Developer/CommandLineTools/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/Library/Developer/CommandLineTools/usr/bin/g++")

set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0 -DDEBUG")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
/usr/local/include
)

link_directories(${easyb_BINARY_DIR} 
/usr/local/lib
)

set(puzzle_src ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(puzzle ${puzzle_src})
target_link_libraries(puzzle pthread)


