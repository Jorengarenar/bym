#pragma once

#include <string>

#include <ncurses.h>

class Cmd {
  public:
    Cmd();
    ~Cmd();
    void redraw();
    int operator ()();
    void error(const char*);

  private:
    std::string input();
    WINDOW* line;
};
