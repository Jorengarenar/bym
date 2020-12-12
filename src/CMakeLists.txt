set(objs "util;buffer;window;cli;editor;parser;options;mapping")

foreach(obj ${objs})
  add_library(${obj} OBJECT ${obj}.cpp)
  target_link_libraries(${obj} ${CURSES_LIBRARIES} ${JCBCPP})
endforeach()

add_executable("${CMAKE_PROJECT_NAME}" main.cpp ${objs})

target_link_libraries("${CMAKE_PROJECT_NAME}" ${CURSES_LIBRARIES} ${JCBCPP})
