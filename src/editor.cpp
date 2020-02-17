#include "editor.hpp"

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
