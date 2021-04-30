#pragma once

#ifdef BOOST_TEST_DYN_LINK
#    include <boost/test/unit_test.hpp>
#else
#    include <boost/test/included/unit_test.hpp>
#endif

#include <boost/test/tools/output_test_stream.hpp>

boost::test_tools::output_test_stream output;

#define CHECK_OUTPUT(msg) BOOST_CHECK(output.is_equal(msg))

// Because ncurses definies `timeout` as macro!
using boost_timeout = boost::unit_test::timeout;


#include <curses.h>

void unget_string_to_ncurses(std::string s)
{
    for (auto i = s.rbegin(); i != s.rend(); ++i) {
        ungetch(*i);
    }
}
