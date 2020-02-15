#include "window.hpp"

Window::Window(short h, short w, Buffer& buf)
{
    height = h;
    width  = w;
    buffer = &buf;
    win    = newwin(h, w, 0, 0);
}

Window::~Window()
{
    delwin(win);
}

void Window::buf(Buffer& b)
{
    buffer = &b;
    fill();
}

void Window::fill()
{
    wresize(win, LINES, COLS);
    buffer->print(win);
    wrefresh(win);
}
