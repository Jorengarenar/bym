#include "cmd.hpp"

#include <ncurses.h>

#include "util.hpp"

Cmd::Cmd()
{
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

Cmd::~Cmd()
{
    delwin(line);
}

void Cmd::redraw()
{
    delwin(line);
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

int Cmd::operator ()()
{
    char buf[1000];
    mvwprintw(line, 0, 0, ":");
    wrefresh(line);
    {
        EnableCursor x;
        wgetstr(line, buf);
    }
    return 1;
}
