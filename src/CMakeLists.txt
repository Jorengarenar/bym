set(objs "util;buffer;window;cmd;editor")

add_library(util   OBJECT util.cpp)
add_library(buffer OBJECT buffer.cpp)
add_library(window OBJECT window.cpp util)
add_library(cmd    OBJECT cmd.cpp    util)
add_library(editor OBJECT editor.cpp)
add_library(parser OBJECT parser.cpp)

foreach(obj ${objs})
  target_link_libraries(${obj} ${CURSES_LIBRARIES})
endforeach()

list(APPEND objs parser)

add_executable("${CMAKE_PROJECT_NAME}" main.cpp ${objs})

target_link_libraries("${CMAKE_PROJECT_NAME}" ${CURSES_LIBRARIES})
