#include "window.hpp"

Window::Window(short h, short w, Buffer& buf)
{
    height = h;
    width  = w;
    buffer = &buf;
    win    = newwin(height, width, 0, 0);
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
    buffer->print(win);
    wrefresh(win);
}
