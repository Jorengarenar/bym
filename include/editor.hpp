#pragma once

#include <vector>

#include "buffer.hpp"
#include "window.hpp"
#include "cli.hpp"
#include "parser.hpp"

/// Editor
///
/// Contains main functionalities of program
class Editor {
    Editor() = default;
public:
    Editor(const Editor&)         = delete;
    void operator=(const Editor&) = delete;

    static Editor& getInstance()
    {
        static Editor instance;
        return instance;
    }

private:

    /// Initializes curses
    struct InitCurses {
        InitCurses();
        ~InitCurses();
    } initCurses;

    std::vector<Buffer> buffers; ///< List of opened buffers
    std::vector<Window> windows; ///< List of existing windows

    void replaceByte(); ///< Wrapper over cw->replaceByte()

public:
    Window* cw; ///< Current window
    Cli cli; ///< Command line
    Parser parser; ///< Parses configs and commands

    void init(int argc, char* argv[], int optind); // yes, copy of main() arguments
    ~Editor() = default;

    bool input(); ///< Handles input (for main loop)
};

#define Editor() Editor::getInstance()
