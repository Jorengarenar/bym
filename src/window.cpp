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
        .hex     = newwin(height, cols*3, 0, 10),
        .text    = newwin(height, cols+5, 0, cols*3 + 10),
        .statusline = nullptr
    };
    refresh();

    fill();
    updateStatusLine();
    placeCursor(x, y, current);
}

Window::~Window()
{
    delwin(SubWindows.hex);
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
    if (COLS > 20) {
        height = LINES-1;
        width  = COLS;
        cols   = width/4 - 4;

        delwin(SubWindows.hex);
        delwin(SubWindows.text);
        delwin(SubWindows.numbers);
        delwin(SubWindows.statusline);
        SubWindows = {
            .numbers = newwin(height, 10, 0, 0),
            .hex     = newwin(height, cols*3, 0, 10),
            .text    = newwin(height, cols+5, 0, cols*3 + 10),
            .statusline = nullptr
        };
        refresh();

        fill();
        updateStatusLine();

        if (x != current % cols && current % cols) {
            y = current / cols;
            x = current % cols;
        }
        placeCursor(y, x, current);
    }
}

void Window::fill()
{
    buffer->print(SubWindows.hex, SubWindows.numbers, SubWindows.text, getmaxy(SubWindows.hex)-1, cols);
    wrefresh(SubWindows.hex);
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

void Window::hjkl(Direction direction)
{
    short x_prev = x;
    short y_prev = y;
    size_t current_prev = current;

    if (direction == Direction::down && current + cols < buffer->size()) {
        current += cols;
        y++;
    }
    else if (direction == Direction::up && current - cols < buffer->size()) {
        current -= cols;
        y--;
    }
    else if (direction == Direction::right && current + 1 < buffer->size() && x < cols - 1) {
        current++;
        x++;
    }
    else if (direction == Direction::left && current > 0 && x > 0) {
        current--;
        x--;
    }

    if (current_prev != current) {
        moveCursor(y, x, current, y_prev, x_prev, current_prev);
        updateStatusLine();
    }
}

void Window::placeCursor(short y, short x, size_t current)
{
    wattron(SubWindows.text, A_REVERSE);
    if (buffer->bytes[current] >= 32 && buffer->bytes[current] <= 126) {
        mvwprintw(SubWindows.text, y, x, "%c", buffer->bytes[current]);
    }
    else {
        mvwprintw(SubWindows.text, y, x, " ");
    }
    wattroff(SubWindows.text, A_REVERSE);

    wrefresh(SubWindows.text);

    wattron(SubWindows.hex, A_REVERSE);
    mvwprintw(SubWindows.hex, y, x*3, "%02X", buffer->bytes[current]);
    wattroff(SubWindows.hex, A_REVERSE);

    wrefresh(SubWindows.hex);
}

void Window::moveCursor(short y, short x, size_t current, short y_prev, short x_prev, size_t current_prev)
{
    if (buffer->bytes[current_prev] >= 32 && buffer->bytes[current_prev] <= 126) {
        mvwprintw(SubWindows.text, y_prev, x_prev, "%c", buffer->bytes[current_prev]);
    }
    else {
        mvwprintw(SubWindows.text, y_prev, x_prev, " ");
    }
    mvwprintw(SubWindows.hex, y_prev, x_prev*3, "%02X", buffer->bytes[current_prev]);

    placeCursor(y, x, current);
}

// vim: fen
