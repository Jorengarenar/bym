#include <ncurses.h>
#include <vector>

#include "buffer.hpp"
#include "editor.hpp"
#include "window.hpp"

bool handleInput(Window& w)
{
    wchar_t c = getch();

    switch (c) {
        case KEY_RESIZE:
            refresh();
            w.redraw();
            break;
        case 'q':
            return false;
            break;
        case 'j':
            w.hjkl(Direction::down);
            break;
        case 'k':
            w.hjkl(Direction::up);
            break;
        case 'l':
            w.hjkl(Direction::right);
            break;
        case 'h':
            w.hjkl(Direction::left);
            break;
        case 'n':
            break;
    }
    return true;
}

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
