#pragma once

#include <functional>
#include <ncurses.h>

#include "buffer.hpp"

enum class Direction { down, up, right, left };

class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    short  height;
    short  width;
    short  cols;
    size_t current;

    void buf(Buffer&);
    void updateStatusLine();
    void redraw(short = LINES-1, short = COLS);
    void replaceByte();
    void applyToSubWindows(std::function<void (WINDOW*)>);
    void save();

    void hjkl(Direction);

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } subWindows;

  private:
    short   y;
    short   x;
    Buffer* buffer;

    void fill();
    void genSubWindows();
    void delSubWindows();

    void placeCursor();
    template<typename T, typename R> void moveCursor(T, T, R, T, T, R);
};
