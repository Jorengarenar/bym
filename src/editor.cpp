#include "editor.hpp"

#include <ncurses.h>

#include "util.hpp"


Editor::InitCurses::InitCurses()
{
    if (!initscr()) {
        exit(1);
    }

    curs_set(FALSE);
    keypad(stdscr, TRUE);
    noecho();
    nocbreak();
    raw();
}

Editor::InitCurses::~InitCurses()
{
    endwin();
}


void Editor::init(int argc, char* argv[], int optind)
{
    parser.config();

    if (argc > optind) {
        buffers.assign(argv+optind, argv+argc);
    }
    else {
        buffers.emplace_back();
    }

    windows.emplace_back(LINES-1, COLS, buffers[0]);
    cw = &windows.front();
}

void Editor::replaceByte()
{
    switch (cw->replaceByte()) {
        case 1:
            cli.error("Buffer size is 0");
            break;
    }
}

bool Editor::input()
{
    int c = getchar();

    switch (c) {
        case KEY_RESIZE:
            cw->redraw();
            cli.redraw();
            break;
        case 'h':
            cw->mvCursor(Direction::left);
            break;
        case 'j':
            cw->mvCursor(Direction::down);
            break;
        case 'k':
            cw->mvCursor(Direction::up);
            break;
        case 'l':
            cw->mvCursor(Direction::right);
            break;
        case 'r':
            replaceByte();
            break;
        case ':':
            return cli();
    }
    return true;
}
