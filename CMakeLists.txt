cmake_minimum_required(VERSION 3.10)

project(Bim)

add_definitions(-DPROJECT_NAME="${CMAKE_PROJECT_NAME}")
add_definitions(-DVERSION="0.0.1")
string(TOLOWER "${CMAKE_PROJECT_NAME}" EXECUTABLE_NAME)
add_definitions(-DEXECUTABLE_NAME="${EXECUTABLE_NAME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extern/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(PreventInSourceBuilds)
include(DefineOutputDirs)
include(ExportCompileCommands)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")

find_package(Curses REQUIRED)

include_directories(include/)

add_subdirectory(src/)
set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES
  OUTPUT_NAME "${EXECUTABLE_NAME}")

include(CTest)
add_subdirectory(tests/)
