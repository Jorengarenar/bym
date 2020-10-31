add_definitions(-DBOOST_TEST_DYN_LINK)
include(BoostTestHelper)

set(objs "util;buffer;window;cli;editor;parser;options")

add_boost_test_file(test_buffer.cpp ${objs})

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
