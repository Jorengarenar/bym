#include "editor.hpp"

int Editor::loop()
{
    while (true) {
        int c = input();

        for (auto a: mappings(c)) {
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

                case Action::NEXTBYTE:
                    cw->gotoByte(cw->currentByte + 1);
                    break;

                case Action::FIRSTCOL:
                    cw->mvCursor(0, cw->y);
                    break;
                case Action::LASTCOL:
                    cw->mvCursor(cw->opts.cols() - 1, cw->y);
                    break;
                case Action::FIRSTBYTE:
                    cw->gotoByte(0);
                    break;
                case Action::LASTBYTE:
                    cw->gotoByte(cw->buffer->size() - 1);
                    break;

                case Action::CMD:
                    if (!cli()) {
                        return 0;
                    }
                    break;
                case Action::SEARCH:
                    break;

                case Action::REPLACE:
                    replaceByte();
                    break;
                case Action::INCREMENT:
                    cw->addToByte(1);
                    break;
                case Action::DECREMENT:
                    cw->addToByte(-1);
                    break;

                case Action::SAVEQUIT:
                    cw->save();
                    [[fallthrough]];
                case Action::QUIT:
                    return 0;

                default:
                    break;
            }
        }
    }
}

// vim: fdl=4
