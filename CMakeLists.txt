cmake_minimum_required(VERSION 3.10)

project(bVim)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extern/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(PreventInSourceBuilds)
include(DefineOutputDirs)
include(ExportCompileCommands)

set(CMAKE_CXX_STANDARD 17)
find_package(Curses REQUIRED)
add_compile_options(-g -Wall -Wextra -pedantic)

include_directories(include/)
add_subdirectory(src/)

include(CTest)
add_subdirectory(tests/)
