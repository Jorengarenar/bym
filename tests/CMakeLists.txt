add_definitions(-DBOOST_TEST_DYN_LINK)
include(BoostTestHelper)

add_boost_test_file(test_buffer.cpp buffer window)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
