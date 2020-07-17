#include "editor.hpp"

bool handleInput(Window& w, Cmd& cmd)
{
    wchar_t c = getch();

    switch (c) {
        case KEY_RESIZE:
            w.redraw();
            cmd.redraw();
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
            return cmd();
            break;
    }
    return true;
}
