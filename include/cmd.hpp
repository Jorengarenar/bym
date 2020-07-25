#pragma once

#include <string>

#include <ncurses.h>

class Editor;

class Cmd {
    std::string input();
    WINDOW* line;
    Editor& editor;
    bool complete(std::string&, int&, int&);
public:
    Cmd(Editor&);
    ~Cmd();
    void redraw();
    bool operator()();
    void error(std::string);
};
