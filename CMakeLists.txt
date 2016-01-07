/*

The MIT License (MIT)

Copyright (c) 2016 David Morton

https://github.com/damorton/raig.git

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
cmake_minimum_required(VERSION 2.8)
project(raig)
include(ExternalProject)

ExternalProject_Add(libsocket
    PREFIX ${CMAKE_BINARY_DIR}/build/libsocket
    GIT_REPOSITORY https://github.com/damorton/libsocket.git    
    BINARY_DIR ${CMAKE_BINARY_DIR}/external/libsocket
    SOURCE_DIR ${CMAKE_BINARY_DIR}/external/libsocket
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(UNIX AND NOT APPLE)
  set(PROJECT_LINK_LIBS libsocket.so)
else()
  set(PROJECT_LINK_LIBS libsocket.dylib)
endif()

set(SOURCES src/ai_manager.cc src/main.cc src/network_manager.cc src/pathfinding_astar.cc include/pathfinding_astar.h include/game_world.h include/search_cell.h include/vector3.h)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/)

link_directories(${PROJECT_BINARY_DIR}/lib/)

add_executable(raig ${SOURCES})

add_dependencies(raig libsocket)

target_link_libraries(raig ${PROJECT_LINK_LIBS})
