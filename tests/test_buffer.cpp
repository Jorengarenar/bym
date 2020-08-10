#define BOOST_TEST_MODULE BufferTests
#include "init_tests.hpp"

#include "buffer.hpp"

BOOST_AUTO_TEST_SUITE(nofile);

BOOST_AUTO_TEST_CASE(size)
{
    Buffer f;
    BOOST_CHECK_EQUAL(f.size(), 0);
}

BOOST_AUTO_TEST_CASE(empty)
{
    Buffer f;
    BOOST_CHECK(f.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(testfile256);

BOOST_AUTO_TEST_CASE(size)
{
    Buffer f{ "res/testfile" };
    BOOST_CHECK_EQUAL(f.size(), 256);
}

BOOST_AUTO_TEST_CASE(empty)
{
    Buffer f{ "res/testfile" };
    BOOST_CHECK(!f.empty());
}

BOOST_AUTO_TEST_SUITE_END();

// vim: fen
