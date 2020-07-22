#pragma once

#include <vector>

#include "buffer.hpp"
#include "window.hpp"
#include "cmd.hpp"

class Editor {
    struct InitCurses {
        InitCurses();
        ~InitCurses();
    } initCurses;

    std::vector<Buffer> buffers;
    std::vector<Window> windows;
    Window* cw; // current window
    Cmd cmd;
    void replaceByte(); // wrapper over cw->replaceByte()

public:
    Editor(int argc, char* argv[], int optind); // yes, copy of main() arguments
    ~Editor() = default;

    bool operator()(); // handle input (for main loop)

    friend bool parse(Editor&, std::string);
};
