#define BOOST_TEST_MODULE EditorTests
#include "init_tests.hpp"

#include "editor.hpp"


struct F1 {
    F1()
    {
        const char* file[] = { "res/testfile" };
        Editor().init(1, (char**)file, 0);
    }

    void do_editor(int c)
    {
        unget_string_to_ncurses(":q\n");
        unget_wch(c);
        Editor().loop();
    }

    void do_editor(std::string s)
    {
        unget_string_to_ncurses(":q\n");
        unget_string_to_ncurses(s);
        Editor().loop();
    }

};

struct F2 : F1 {
    F2() : F1() {
        Editor().setOption("cols=16");
    }

    unsigned char b()
    {
        auto buf = *Editor().cw->buffer;
        auto c   = Editor().cw->currentByte;
        return buf[c];
    }
};


BOOST_FIXTURE_TEST_SUITE(exiting, F1)

BOOST_AUTO_TEST_CASE(test0, *boost_timeout(2))
{
    // essentialy the same as fixture's `do_editor` with empty string
    unget_string_to_ncurses(":q\n");
    Editor().loop();
}

BOOST_AUTO_TEST_CASE(test1, *boost_timeout(2))
{
    unget_string_to_ncurses("ZZ");
    Editor().loop();
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(editing, F1)

BOOST_AUTO_TEST_CASE(test1)
{
    do_editor(CTRL('a'));
    auto b = *Editor().cw->buffer;
    BOOST_CHECK(b[0] == 1);
}

BOOST_AUTO_TEST_CASE(test2)
{
    do_editor("lr11");
    auto b = *Editor().cw->buffer;
    BOOST_CHECK(b[1] == 0x11);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(moving, F2)

BOOST_AUTO_TEST_CASE(test1)
{
    do_editor('G');
    BOOST_CHECK(b() == 255);
}

BOOST_AUTO_TEST_CASE(test2)
{
    do_editor("j");
    BOOST_CHECK(b() == 16);
}

BOOST_AUTO_TEST_CASE(test3)
{
    do_editor("$");
    BOOST_CHECK(b() == 15);
}

BOOST_AUTO_TEST_CASE(test4)
{
    do_editor("lll");
    BOOST_CHECK(b() == 3);
    do_editor("0");
    BOOST_CHECK(b() == 0);
}

BOOST_AUTO_TEST_SUITE(newline)

BOOST_AUTO_TEST_CASE(test1)
{
    do_editor("\n");
    BOOST_CHECK(b() == 16);
}

BOOST_AUTO_TEST_CASE(test2)
{
    do_editor("$\n");
    BOOST_CHECK(b() == 16);
}

BOOST_AUTO_TEST_CASE(test3)
{
    do_editor("G\n");
    BOOST_CHECK(b() == 255);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
