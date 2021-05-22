set(objs "util;buffer;window;cli;editor;editor_loop;parser;options;mapping")

set(LIBS ${CURSES_LIBRARIES} ${JCBCPP} ${XDGdirs} ${LUA_LIBRARIES})

foreach(obj ${objs})
  add_library(${obj} OBJECT ${obj}.cpp)
  target_link_libraries(${obj} ${LIBS})
endforeach()

add_executable("${CMAKE_PROJECT_NAME}" main.cpp ${objs})

target_link_libraries("${CMAKE_PROJECT_NAME}" ${LIBS})
