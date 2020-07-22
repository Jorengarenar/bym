#include "util.hpp"

#include <cctype>
#include <cstdlib>

#include <ncurses.h>

EnableCursor::EnableCursor()
{
    curs_set(TRUE);
}

EnableCursor::~EnableCursor()
{
    curs_set(FALSE);
}

char toPrintable(int x)
{
    if (isprint(x)) {
        return x;
    }
    else {
        return ' ';
    }
}
