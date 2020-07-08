#include "util.hpp"

EnableCursor::EnableCursor()
{
    curs_set(TRUE);
    echo();
}

EnableCursor::~EnableCursor()
{
    curs_set(FALSE);
    noecho();
}
