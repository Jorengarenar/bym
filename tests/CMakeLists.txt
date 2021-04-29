include(BoostTestHelper)
if(BOOST_FOUND)
  add_definitions(-DBOOST_TEST_DYN_LINK)

  set(objs "util;buffer;window;cli;editor;editor_loop;parser;options;mapping")

  add_boost_test_file(test_buffer.cpp ${objs})
  add_boost_test_file(test_window.cpp ${objs})
  add_boost_test_file(test_editor.cpp ${objs})

  file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
