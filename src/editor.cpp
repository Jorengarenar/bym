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

    for (auto& b: buffers) {
        windows.push_back(std::make_shared<Window>(b));
    }
    cw = windows[0];

    statusline = newwin(1, COLS, LINES-2, 0);
    tabline    = newwin(1, COLS, 0, 0);
    updateStatusline();
    updateTabline();
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

void Editor::updateStatusline()
{
    if (!cw) { return; }
    auto& b = *(cw->buffer);
    mvwprintw(statusline, 0, 0,
              "Size: %zu B  |  Byte: 0x%02X",
              b.size(), b[cw->currentByte]); // TODO
    wclrtoeol(statusline);
    wrefresh(statusline);
}

void Editor::updateTabline()
{
    wmove(tabline, 0, 0);
    for (auto w: windows) {
        if (w == cw) {
            wattron(tabline, A_REVERSE);
        }
        wprintw(tabline, " %s ", w->buffer->path.c_str());
        wattroff(tabline, A_REVERSE);
    }
    wrefresh(tabline);
}

void Editor::newWin() // TODO
{
    cw->delSubWindows();
    windows.push_back(std::make_shared<Window>(buffers[1]));
    switchWin(1);
}

std::size_t Editor::winNr()
{
    std::size_t i = 0;
    while (windows[i++] != cw); // How many CWE do you want in one line? Yes.
    return i-1;
}

void Editor::switchWin(int n)
{
    cw->delSubWindows();
    cw = windows.at(n);
    updateTabline();
    cw->redraw();
}
