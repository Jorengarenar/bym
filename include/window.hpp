#pragma once

#include <functional>

#include <ncurses.h>

#include "buffer.hpp"

enum class Direction { DOWN, UP, RIGHT, LEFT };

/// Window
///
/// Edits and displays buffers
class Window {
public:
    Window(int, int, Buffer&);
    ~Window();

    unsigned short height; ///< Height of window
    unsigned short width;  ///< Width of window
    std::size_t currentByte;    ///< Byte cursor is currently over
    std::size_t prevByte;

    std::size_t startline;

    void buf(Buffer&);                                      ///< Change buffer in window
    void updateStatusLine();                                ///< Updates status line
    void redraw(short = LINES-1, short = COLS);             ///< Redraws window
    void applyToSubWindows(std::function<void (WINDOW*)>);  ///< Apply function to subwindows
    void save();                                            ///< Save currently loaded buffer

    int replaceByte(); ///< Replaces value of byte

    void mvCursor(Direction); ///< Movement
    void mvCursor(std::size_t, std::size_t); ///< Movement

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } subWindows;

    Buffer* buffer;
    int y; ///< Current line on screen
    int x; ///< Current column (current byte in column) on screen

private:
    void genSubWindows(); ///< Generate subwindows
    void delSubWindows(); ///< Delete subwindows

    bool inputByte(char*); ///< Get byte value

    void print(); ///< print buffer content in subWindows

    void placeCursor(); ///< Place cursor on current x,y coordinates

    struct Opts {
        Opts(Window&);
        Window& w;

        unsigned short cols() const;  ///< Maximal number of columns of bytes
        char blank() const;
    } opts;
    friend struct Opts;
};
