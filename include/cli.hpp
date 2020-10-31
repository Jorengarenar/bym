#pragma once

#include <string>

#include <ncurses.h>

/// CLI
///
/// Command line
class Cli {
    std::string input(); ///< Gets command from user
    WINDOW* line; ///< Points to CLI part of the screen
    bool complete(std::string&, int&, int&); ///< Complete commands
public:
    Cli();
    ~Cli();

    void redraw(); ///< Redraw `line`
    bool operator ()(); ///< Get command (`input()`) and pass it to parser
    void error(std::string); ///< Display error message
    void echo(std::string);  ///< Prints message
};
