#pragma once

#include <functional>

#include <ncurses.h>

#include "buffer.hpp"

enum class Direction { down, up, right, left };

class Window {
  public:
    Window(short, short, Buffer&);
    ~Window();

    unsigned short  height;
    unsigned short  width;
    unsigned short  cols;   // maximal number of columns of bytes
    size_t currentByte;

    void buf(Buffer&); // change buffer in window
    void updateStatusLine();
    void redraw(short = LINES-1, short = COLS);
    void replaceByte();
    void applyToSubWindows(std::function<void (WINDOW*)>);
    void save(); // save actually loaded buffer

    void hjkl(Direction);

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } subWindows;

  private:
    unsigned short y; // current line
    unsigned short x; // current column (current byte in column)
    Buffer* buffer;

    void fill(); // print buffer content in subWindows
    void genSubWindows();
    void delSubWindows();

    void placeCursor();
    template<typename T, typename R> void moveCursor(T, T, R);
};
