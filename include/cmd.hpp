#pragma once

#include <ncurses.h>

class Cmd {
  public:
    Cmd();
    ~Cmd();
    void redraw();

  private:
    WINDOW* line;
};
