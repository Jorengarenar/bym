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
    cbreak();
}

int main(int argc, char* argv[])
{
    std::vector<Buffer> buffers;
    if (argc > 1) {
        buffers.assign(argv+1, argv+argc);
    }
    else {
        buffers.push_back(Buffer());
    }

    init_curses();

    Window w(LINES-1, COLS, buffers[0]);
    Cmd cmd;

    while (handleInput(w, cmd));

    endwin();
}

// vim: fen
