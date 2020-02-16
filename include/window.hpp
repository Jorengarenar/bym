#pragma once

#include <ncurses.h>

#include "buffer.hpp"

class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    size_t current;

    void buf(Buffer&);
    void updateStatusLine();
    void redraw();

    void moveDown();
    void moveUp();
    void moveRight();
    void moveLeft();

  private:
    short   height;
    short   width;
    short   cols;
    short   y;
    short   x;
    Buffer* buffer;

    struct {
        WINDOW* numbers;
        WINDOW* hexs;
        WINDOW* text;
        WINDOW* statusline;
    } SubWindows;

    void fill();
};
