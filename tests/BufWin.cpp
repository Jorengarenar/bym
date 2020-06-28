#define BOOST_TEST_MODULE BuffersAndWinsows
#include "init_tests.hpp"

#include "buffer.hpp"
#include "window.hpp"

BOOST_AUTO_TEST_SUITE(buffer)

BOOST_AUTO_TEST_CASE(nofile)
{
    Buffer f;
    BOOST_CHECK_EQUAL(f.size(), 0);
}

BOOST_AUTO_TEST_CASE(testfile256)
{
    Buffer f{"res/testfile"};
    BOOST_CHECK_EQUAL(f.size(), 256);
}

BOOST_AUTO_TEST_SUITE_END()
