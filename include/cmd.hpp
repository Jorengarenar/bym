#pragma once

#include <ncurses.h>

class Cmd {
  public:
    Cmd();
    ~Cmd();
    void redraw();
    int operator ()();

  private:
    WINDOW* line;
};
