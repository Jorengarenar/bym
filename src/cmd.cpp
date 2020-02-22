#include "cmd.hpp"

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
