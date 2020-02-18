#include "editor.hpp"

bool handleInput(Window& w, WINDOW* cmd)
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
        case 'h':
            w.hjkl(Direction::left);
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
        case 'r':
            w.replaceByte();
            break;
        case ':':
            wclear(cmd);
            wmove(cmd, 0, 0);
            wprintw(cmd, ": ");
            curs_set(TRUE);
            wrefresh(cmd);
            echo();
            if (wgetch(cmd) == 's' && wgetch(cmd) == 10) {
                w.save();
            }
            curs_set(FALSE);
            noecho();
    }
    return true;
}
