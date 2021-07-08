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

    windows.emplace_back(buffers[0]);
    cw = &windows.front();

    statusline = newwin(1, COLS, LINES-2, 0);
    updateStatusLine();
}

void Editor::replaceByte()
{
    switch (cw->replaceByte()) {
        case 1:
            cli.error("Buffer size is 0");
            break;
    }
}

int Editor::input()
{
    timeout(2000);
    return getch();
}

void Editor::setOption(std::string opt)
{
    options.set(opt);
    for (auto& b: buffers) {
        b.options.set(opt);
    }
}

void Editor::updateStatusLine()
{
    if (!cw) { return; }
    mvwprintw(statusline, 0, 0, "%zu", cw->buffer->size()); // TODO
    wclrtoeol(statusline);
    wrefresh(statusline);
}
