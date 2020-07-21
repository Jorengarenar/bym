#pragma once

#include <ncurses.h>

class Cmd {
  public:
    Cmd();
    ~Cmd();
    void redraw();
    int operator ()();
    void error(const char*);

  private:
    WINDOW* line;
};
