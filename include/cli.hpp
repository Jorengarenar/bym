#pragma once

#include <string>

#include <ncurses.h>

class Editor;

class Cli {
    std::string input();
    WINDOW* line;
    Editor& editor;
    bool complete(std::string&, int&, int&);
public:
    Cli(Editor&);
    ~Cli();
    void redraw();
    bool operator()();
    void error(std::string);
};
