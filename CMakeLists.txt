cmake_minimum_required(VERSION 2.8)
project(raig)
include(ExternalProject)

ExternalProject_Add(libsocket
    PREFIX ${CMAKE_BINARY_DIR}/build/libsocket
    GIT_REPOSITORY https://github.com/damorton/libsocket.git
    INSTALL_COMMAND ""
    BINARY_DIR ${CMAKE_BINARY_DIR}/external/libsocket
    SOURCE_DIR ${CMAKE_BINARY_DIR}/external/libsocket
)

ExternalProject_Add(libraig
   PREFIX ${CMAKE_BINARY_DIR}/build/libraig
   GIT_REPOSITORY https://github.com/damorton/libraig.git
   INSTALL_COMMAND ""
   BINARY_DIR ${CMAKE_BINARY_DIR}/external/libraig
   SOURCE_DIR ${CMAKE_BINARY_DIR}/external/libraig
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(UNIX AND NOT APPLE)
  set(PROJECT_LINK_LIBS libsocket.so libraig.so)
else()
  set(PROJECT_LINK_LIBS libsocket.dylib libraig.dylib)
endif()

set(SOURCES src/AIManager.cc src/main.cc src/NetworkManager.cc)

link_directories(external/libraig/)
link_directories(external/libsocket/)

add_executable(raig ${SOURCES})

add_dependencies(raig libsocket libraig)

target_link_libraries(raig ${PROJECT_LINK_LIBS})
