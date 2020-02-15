#include "window.hpp"

Window::Window(short h, short w, Buffer& buf)
{
    height     = h;
    width      = w;
    cols       = width/4 - 4;
    buffer     = &buf;
    win        = newwin(h, w, 0, 0);
    statusline = nullptr;
    current    = 0;
    curr_col   = 0;
    y          = 0;
    x          = 11;
    fill();
    updateStatusLine();
    wmove(win, 0, 11);
    wrefresh(win);
}

Window::~Window()
{
    delwin(statusline);
    delwin(win);
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
    wresize(win, height, width);
    fill();
    updateStatusLine();
}

void Window::fill()
{
    buffer->print(win, getmaxy(win)-1, cols);
    wrefresh(win);
}

void Window::updateStatusLine()
{
    if (statusline) {
        delwin(statusline);
    }
    statusline = newwin(1, width, height-1, 0);
    wprintw(statusline, "test");
    wrefresh(statusline);
}

void Window::moveDown()
{
    if (current + cols <= buffer->size()) {
        current += cols;
        move(++y, x);
    }
}

void Window::moveUp()
{
    if (current - cols <= buffer->size()) {
        current -= cols;
        move(--y, x);
    }
}

void Window::moveRight()
{
    if (current < buffer->size() && curr_col < cols - 1) {
        current++;
        curr_col++;
        x += 3;
        move(y, x);
    }
}

void Window::moveLeft()
{
    if (current > 0 && curr_col > 0) {
        current--;
        curr_col--;
        x -= 3;
        move(y, x);
    }
}

// vim: fen
