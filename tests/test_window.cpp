#define BOOST_TEST_MODULE BufferTests
#include "init_tests.hpp"

#include "window.hpp"

struct F {
    Buffer b1;
    Buffer b2;
    Window w1;
    Window w2;
    F() : b1{"res/testfile"}, w1{b1}, w2{b2} {}
};


BOOST_FIXTURE_TEST_SUITE(foo, F)


BOOST_AUTO_TEST_SUITE(editing)


BOOST_AUTO_TEST_CASE(test1)
{
    unget_string_to_ncurses("11");
    w1.replaceByte();
    BOOST_CHECK(b1[0] == 0x11);
}

BOOST_AUTO_TEST_CASE(test2)
{
    BOOST_CHECK(w2.replaceByte() == 1);
}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(moving)


/*
BOOST_AUTO_TEST_CASE(test1)
{
    w1.mvCursor(0,1);
    BOOST_CHECK(w1.currentByte == 20);
}

BOOST_AUTO_TEST_CASE(test2)
{
    w1.mvCursor(2,2);
    BOOST_CHECK(w1.currentByte == 42);
}

BOOST_AUTO_TEST_CASE(test3)
{
    w1.mvCursor(2,2);
    BOOST_CHECK(b1[w1.currentByte] == 42);
}
*/

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()
