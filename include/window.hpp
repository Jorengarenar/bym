#pragma once

#include <functional>

#include <ncurses.h>

#include "buffer.hpp"

enum class Direction { down, up, right, left };

/// Window
///
/// Edits and displays buffers
class Window {
public:
    Window(int, int, Buffer&);
    ~Window();

    unsigned short height; ///< Height of window
    unsigned short width;  ///< Width of window
    unsigned short cols;   ///< Maximal number of columns of bytes
    size_t currentByte;    ///< Byte cursor is currently over


    void buf(Buffer&);                                      ///< Change buffer in window
    void updateStatusLine();                                ///< Updates status line
    void redraw(short = LINES-1, short = COLS);             ///< Redraws window
    void applyToSubWindows(std::function<void (WINDOW*)>);  ///< Apply function to subwindows
    void save();                                            ///< Save currently loaded buffer

    int replaceByte(); ///< Replaces value of byte

    void hjkl(Direction); ///< Movement

    struct {
        WINDOW* numbers;
        WINDOW* hex;
        WINDOW* text;
        WINDOW* statusline;
    } subWindows;

private:
    int y; ///< Current line
    int x; ///< Current column (current byte in column)
    Buffer* buffer;

    void genSubWindows(); ///< Generate subwindows
    void delSubWindows(); ///< Delete subwindows

    bool inputByte(char*); ///< Get byte value

    void print(short = 0); ///< print buffer content in subWindows

    void placeCursor(); ///< Place cursor on current x,y coordinates
    template<typename T, typename R> void moveCursor(T, T, R); ///< Move cursor
};
