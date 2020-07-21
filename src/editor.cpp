#include "editor.hpp"

void replaceByte(Window& w, Cmd& cmd)
{
    switch (w.replaceByte()) {
        case 1:
            cmd.error("Buffer size is 0");
    }
}

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
            replaceByte();
            break;
        case ':':
            switch (cmd()) {
                case 0:
                    return false;
                    break;
                case 1:
                    w.save();
                    break;
            }
            break;
    }
    return true;
}
