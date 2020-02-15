#pragma once

#include <ncurses.h>

#include "buffer.hpp"


class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    void buf(Buffer&);
    void updateStatusLine();
    void resize();

  private:
    short   height;
    short   width;
    Buffer* buffer;
    WINDOW* win;
    WINDOW* statusline;

    void fill();
};
