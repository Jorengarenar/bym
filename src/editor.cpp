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
    cbreak();
}

Editor::InitCurses::~InitCurses()
{
    endwin();
}

Editor::Editor(int argc, char* argv[], int optind)
{
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
            cmd.error("Buffer size is 0");
            break;
    }
}

bool Editor::operator ()()
{
    int c = getc(stdin);

    switch (c) {
        case KEY_RESIZE:
            cw->redraw();
            cmd.redraw();
            break;
        case 'h':
            cw->hjkl(Direction::left);
            break;
        case 'j':
            cw->hjkl(Direction::down);
            break;
        case 'k':
            cw->hjkl(Direction::up);
            break;
        case 'l':
            cw->hjkl(Direction::right);
            break;
        case 'r':
            replaceByte();
            break;
        case ':':
            switch (cmd()) {
                case 0:
                    return false;
                    break;
                case 1:
                    cw->save();
                    break;
            }
            break;
    }
    return true;
}
