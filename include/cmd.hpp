#pragma once

#include <string>

#include <ncurses.h>

class Editor;

class Cmd {
    std::string input();
    WINDOW* line;
    Editor& editor;
public:
    Cmd(Editor&);
    ~Cmd();
    void redraw();
    bool operator()();
    void error(std::string);
};
