#
# Raig
# 
# David Morton 2016
#
cmake_minimum_required(VERSION 2.8)
project(raig)
include(ExternalProject)

ExternalProject_Add(libsocket
    PREFIX ${CMAKE_BINARY_DIR}/build/libsocket
    GIT_REPOSITORY git@bitbucket.org:damorton/libsocket.git
    GIT_TAG master 
    BINARY_DIR ${CMAKE_SOURCE_DIR}/lib/libsocket
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/libsocket
    INSTALL_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories(	
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/src
)

set(HEADERS
    src/ai/ai_manager.h
    src/ai/pathfinding_astar.h
    src/net/network_manager.h
    src/base/game_world.h
    src/base/vector3.h
    src/base/search_cell.h
)

set(SOURCES
	src/ai/ai_manager.cc
	src/ai/pathfinding_astar.cc	 
	src/net/network_manager.cc 
	src/base/game_world.cc
	src/base/vector3.cc	 
	src/base/game_world.cc
)

add_library(raig ${SOURCES} ${HEADERS})

add_dependencies(raig libsocket)

set_target_properties(raig
PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)



# Link to the libsocket library
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsocket/lib/
)

# Set executable name
set(EXECUTABLE Server)

# Create binary
add_executable(${EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/raig/raig_server.cc)

target_link_libraries(${EXECUTABLE} raig socket)

set_target_properties(${EXECUTABLE}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)



# GTest
set(UNIT_TEST_EXECUTABLE runUnitTests)
add_subdirectory(lib/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES
${CMAKE_SOURCE_DIR}/src/test/test_Vector3.cpp
)

add_executable(${UNIT_TEST_EXECUTABLE} ${TEST_SOURCE_FILES})

add_dependencies(${UNIT_TEST_EXECUTABLE} libsocket)

target_link_libraries(${UNIT_TEST_EXECUTABLE} raig socket gtest gtest_main)

add_test(NAME test COMMAND ${UNIT_TEST_EXECUTABLE})

set_target_properties(${UNIT_TEST_EXECUTABLE}
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test
)

