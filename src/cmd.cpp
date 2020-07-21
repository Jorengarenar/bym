#include "cmd.hpp"

#include <ncurses.h>

#include "parser.hpp"
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
    const int n = 10000;
    char buf[n];
    wclear(line);
    mvwprintw(line, 0, 0, ":");
    wrefresh(line);
    {
        EnableCursor x;
        wgetnstr(line, buf, n);
    }
    return parse(buf);
}

void Cmd::error(const char* msg)
{
    wclear(line);
    mvwprintw(line, 0, 0, msg);
    wrefresh(line);
}
