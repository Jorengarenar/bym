set(objs "buffer;cmd;editor;window;util")

add_library(buffer OBJECT buffer.cpp)
add_library(util   OBJECT util.cpp)
add_library(cmd    OBJECT cmd.cpp    util)
add_library(window OBJECT window.cpp util)
add_library(editor OBJECT editor.cpp)

foreach(obj ${objs})
  target_link_libraries(${obj} ${CURSES_LIBRARIES})
endforeach()

add_executable(bvim main.cpp ${objs})

target_link_libraries(bvim ${CURSES_LIBRARIES})
