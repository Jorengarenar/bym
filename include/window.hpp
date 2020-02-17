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

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } subWindows;

  private:
    short   height;
    short   width;
    short   cols;
    short   y;
    short   x;
    Buffer* buffer;

    void fill();
    void genSubWindows();
    void delSubWindows();

    template<typename T, typename R> void placeCursor(T, T, R);
    template<typename T, typename R> void moveCursor(T, T, R, T, T, R);
};
