#include "window.hpp"

Window::Window(short h, short w, Buffer& buf)
{
    height     = h;
    width      = w;
    cols       = width/4 - 4;
    buffer     = &buf;
    current    = 0;
    y          = 0;
    x          = 0;

    SubWindows = {
        .numbers = newwin(height, 10, 0, 0),
        .hexs    = newwin(height, cols*3, 0, 10),
        .text    = newwin(height, cols+5, 0, cols*3 + 11),
        .statusline = nullptr
    };
    refresh();

    fill();
    updateStatusLine();

    wattron(SubWindows.text, A_REVERSE | A_BLINK);
    mvwprintw(SubWindows.text, 0, 0, " ");
    wattroff(SubWindows.text, A_REVERSE | A_BLINK);
    wrefresh(SubWindows.text);

    wmove(SubWindows.hexs, y, x);
    wrefresh(SubWindows.hexs);
}

Window::~Window()
{
    delwin(SubWindows.hexs);
    delwin(SubWindows.text);
    delwin(SubWindows.numbers);
    delwin(SubWindows.statusline);
}

void Window::buf(Buffer& b)
{
    buffer = &b;
    fill();
}

void Window::redraw()
{
    height = LINES-1;
    width  = COLS;
    cols   = width/4 - 4;

    delwin(SubWindows.hexs);
    delwin(SubWindows.text);
    delwin(SubWindows.numbers);
    delwin(SubWindows.statusline);
    SubWindows = {
        .numbers = newwin(height, 10, 0, 0),
        .hexs    = newwin(height, cols*3, 0, 10),
        .text    = newwin(height, cols+5, 0, cols*3 + 11),
        .statusline = nullptr
    };

    fill();
    updateStatusLine();

    if (x != current % cols && current % cols) {
        x = current % cols;
        y = current / cols;
        wmove(SubWindows.hexs, y, x*3);
        wrefresh(SubWindows.hexs);
    }
}

void Window::fill()
{
    buffer->print(SubWindows.hexs, SubWindows.numbers, SubWindows.text, getmaxy(SubWindows.hexs)-1, cols);
    wrefresh(SubWindows.hexs);
    wrefresh(SubWindows.text);
    wrefresh(SubWindows.numbers);
}

void Window::updateStatusLine()
{
    if (SubWindows.statusline) {
        delwin(SubWindows.statusline);
    }
    SubWindows.statusline = newwin(1, width, height-1, 0);
    if (current < buffer->size()) {
        wprintw(SubWindows.statusline, "%02X", buffer->bytes[current]);
    }
    wrefresh(SubWindows.statusline);
}

void Window::moveDown()
{
    if (current + cols <= buffer->size()) {
        current += cols;
        wmove(SubWindows.hexs, ++y, x*3);
        updateStatusLine();
        wrefresh(SubWindows.hexs);
    }
}

void Window::moveUp()
{
    if (current - cols <= buffer->size()) {
        current -= cols;
        wmove(SubWindows.hexs, --y, x*3);
        updateStatusLine();
        wrefresh(SubWindows.hexs);
    }
}

void Window::moveRight()
{
    if (current < buffer->size() && x < cols - 1) {
        current++;
        x++;
        wmove(SubWindows.hexs, y, x*3);
        updateStatusLine();
        wrefresh(SubWindows.hexs);
    }
}

void Window::moveLeft()
{
    if (current > 0 && x > 0) {
        current--;
        x--;
        wmove(SubWindows.hexs, y, x*3);
        updateStatusLine();
        wrefresh(SubWindows.hexs);
    }
}

// vim: fen
