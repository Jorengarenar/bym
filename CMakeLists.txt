cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extern/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(Bym
  VERSION 0.0.1
  DESCRIPTION "The hex editor"
  HOMEPAGE_URL "https://joren.ga/bym"
  LANGUAGES CXX)

string(TOLOWER "${CMAKE_PROJECT_NAME}" EXECUTABLE_NAME)

add_definitions(-DPROJECT_NAME="${CMAKE_PROJECT_NAME}")
add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")
add_definitions(-DEXECUTABLE_NAME="${EXECUTABLE_NAME}")

include(PreventInSourceBuilds)
include(DefineOutputDirs)
include(ExportCompileCommands)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")

set(ONLY_DOCS NO CACHE BOOL "Whether to build only docs")

if(NOT ONLY_DOCS)

  find_package(Curses REQUIRED)

  include_directories(include/)

  add_subdirectory(src/)

  include(AddExecutable)

  include(CTest)
  add_subdirectory(tests/)

endif()

add_subdirectory(docs/)
