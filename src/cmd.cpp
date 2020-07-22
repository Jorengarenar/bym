#include "cmd.hpp"

#include <ncurses.h>

#include "parser.hpp"
#include "util.hpp"

Cmd::Cmd(Editor& e) : editor(e)
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

std::string Cmd::input()
{
    std::string buf{};
    int b;
    int y = getcury(line);

    EnableCursor c;
    for (int i = 0; b != '\n'; ) {
        b = wgetch(line);

        switch (b) {
            case '\n':
                break;
            case ::ESC:
            case CTRL('c'):
                return "";
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (i) {
                    mvwdelch(line, y, i);
                    buf.pop_back();
                    --i;
                }
                break;
            default:
                wprintw(line, "%c", b);
                buf += b;
                ++i;
        }

    }

    return buf;
}

bool Cmd::operator ()()
{
    wclear(line);
    mvwprintw(line, 0, 0, ":");
    wrefresh(line);

    std::string buf = input();

    if (buf.empty()) {
        wclear(line);
        wrefresh(line);
        return true;
    }

    return parse(editor, buf);
}

void Cmd::error(std::string msg)
{
    wclear(line);
    mvwprintw(line, 0, 0, msg.c_str());
    wrefresh(line);
}
