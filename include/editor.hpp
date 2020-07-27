#pragma once

#include <vector>

#include "buffer.hpp"
#include "window.hpp"
#include "cli.hpp"
#include "parser.hpp"

class Editor {
    struct InitCurses {
        InitCurses();
        ~InitCurses();
    } initCurses;

    std::vector<Buffer> buffers;
    std::vector<Window> windows;

    void replaceByte(); // wrapper over cw->replaceByte()

public:
    Window* cw; // current window

    Cli cli;
    Parser parser;

    Editor(int argc, char* argv[], int optind); // yes, copy of main() arguments
    ~Editor() = default;

    bool operator()(); // handle input (for main loop)
};
