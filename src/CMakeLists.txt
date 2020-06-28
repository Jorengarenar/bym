set(objs "buffer;cmd;editor;window")

foreach(obj ${objs})
  add_library(${obj} OBJECT ${obj}.cpp)
  target_link_libraries(${obj} ${CURSES_LIBRARIES})
endforeach()

add_executable(bvim main.cpp ${objs})

target_link_libraries(bvim ${CURSES_LIBRARIES})
