#pragma once

#include <ncurses.h>

#include "buffer.hpp"

class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    void fill();
    void buf(Buffer&);

  private:
    short   height;
    short   width;
    Buffer* buffer;
    WINDOW* win;
};
