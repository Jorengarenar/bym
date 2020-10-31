#pragma once

#include <vector>

#include "buffer.hpp"
#include "cli.hpp"
#include "options.hpp"
#include "parser.hpp"
#include "window.hpp"

/// Editor
///
/// Contains main functionalities of program
class Editor {
    Editor() {}
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
    Options options;

    void init(int argc, char* argv[], int optind); // yes, copy of main() arguments
    ~Editor() = default;

    bool input(); ///< Handles input (for main loop)
    void set(std::string);
};

#define Editor() Editor::getInstance()
