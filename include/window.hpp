#pragma once

#include <ncurses.h>

#include "buffer.hpp"

enum class Direction { down, up, right, left };

class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    size_t current;

    void buf(Buffer&);
    void updateStatusLine();
    void redraw();

    void hjkl(Direction);

  private:
    short   height;
    short   width;
    short   cols;
    short   y;
    short   x;
    Buffer* buffer;

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } SubWindows;

    void fill();

    void placeCursor(typeof(y), typeof(x), typeof(current));
    void moveCursor(typeof(y), typeof(x), typeof(current), typeof(y), typeof(x), typeof(current));
};
