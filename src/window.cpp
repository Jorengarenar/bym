#include "window.hpp"

Window::Window(short h, short w, Buffer& buf)
{
    height     = h;
    width      = w;
    buffer     = &buf;
    win        = newwin(h, w, 0, 0);
    statusline = nullptr;
    fill();
    updateStatusLine();
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

void Window::resize()
{
    height = LINES-1;
    width  = COLS;
    wresize(win, height, width);
    fill();
    updateStatusLine();
}

void Window::fill()
{
    buffer->print(win, getmaxy(win)-1, getmaxx(win));
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
