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
    int c = std::getchar();

    for(auto a: mappings(c)) {
        switch (a) {
            case Action::RESIZE:
                cw->redraw();
                cli.redraw();
                break;
            case Action::LEFT:
                cw->mvCursor(Direction::LEFT);
                break;
            case Action::DOWN:
                cw->mvCursor(Direction::DOWN);
                break;
            case Action::UP:
                cw->mvCursor(Direction::UP);
                break;
            case Action::RIGHT:
                cw->mvCursor(Direction::RIGHT);
                break;
            case Action::REPLACE:
                replaceByte();
                break;
            case Action::CMD:
                return cli();
            case Action::FIRSTCOL:
                cw->mvCursor(0, cw->y);
                break;
            default:
                break;
        }
    }
    return true;
}

void Editor::setOption(std::string opt)
{
    options.set(opt);
    for (auto& b: buffers) {
        b.options.set(opt);
    }
}
