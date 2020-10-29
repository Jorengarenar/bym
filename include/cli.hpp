#pragma once

#include <string>

#include <ncurses.h>

class Editor;

/// CLI
///
/// Command line
class Cli {
    std::string input(); ///< Gets command from user
    WINDOW* line; ///< Points to CLI part of the screen
    Editor& editor; ///< Parent of this object
    bool complete(std::string&, int&, int&); ///< Complete commands
public:
    Cli(Editor&);
    ~Cli();

    void redraw(); ///< Redraw `line`
    bool operator ()(); ///< Get command (`input()`) and pass it to parser
    void error(std::string); ///< Display error message
};
