#include <ncurses.h>
#include <vector>

#include "buffer.hpp"
#include "editor.hpp"
#include "window.hpp"

void init_curses()
{
    initscr();

    curs_set(FALSE);
    keypad(stdscr, TRUE);
    noecho();
    raw();
}

int main(int argc, char* argv[])
{

    std::vector<Buffer> buf;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            buf.push_back(Buffer(argv[i]));
        }
    }
    else {
        buf.push_back(Buffer());
    }

    init_curses();

    WINDOW* cmd = newwin(1, COLS, LINES-1, 0);
    wprintw(cmd, "basdjnf");
    wrefresh(cmd);

    Window w(LINES-2, COLS, buf[0]);

    while (handleInput(w));

    endwin();
}

// vim: fen
