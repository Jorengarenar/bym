#include <string>
#include "window.hpp"

Window::Window(short height_, short width_, Buffer& buffer_) :
    height(height_), width(width_),
    buffer(&buffer_),
    cols((width-10)/4),
    x(0), y(0), current(0)
{
    genSubWindows();
    fill();
    updateStatusLine();
    placeCursor();
}

Window::~Window()
{
    applyToSubWindows(delwin);
}

void Window::genSubWindows()
{
    subWindows = {
        .numbers = newwin(height, 10, 0, 0),
        .hex     = newwin(height, cols*3, 0, 10),
        .text    = newwin(height, cols+5, 0, cols*3 + 10),
        .statusline = nullptr
    };
    refresh();
}

void Window::updateStatusLine()
{
    if (subWindows.statusline) {
        delwin(subWindows.statusline);
    }
    subWindows.statusline = newwin(1, width, height-1, 0);
    if (current < buffer->size()) {
        wprintw(subWindows.statusline, "%02X", buffer->bytes[current]);
    }
    wrefresh(subWindows.statusline);
}

void Window::buf(Buffer& b)
{
    buffer = &b;
    fill();
}

void Window::redraw(short height_, short width_)
{
    applyToSubWindows(wclear);
    applyToSubWindows(wrefresh);

    if (COLS > 20) {
        height = height_;
        width  = width_;
        cols   = (width-10)/4;

        applyToSubWindows(delwin);
        genSubWindows();

        fill();
        updateStatusLine();

        x = current % cols;
        y = current / cols;

        placeCursor();
    }
}

void Window::applyToSubWindows(std::function<void (WINDOW*)> f)
{
    f(subWindows.hex);
    f(subWindows.text);
    f(subWindows.numbers);
    f(subWindows.statusline);
}

void Window::fill()
{
    buffer->print(*(this));
    applyToSubWindows(wrefresh);
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

void Window::placeCursor()
{
    bool foo = true;
    unsigned char c;
    if (buffer->size()) {
        c = buffer->bytes[current];
    }
    else {
        c = 0;
        foo = false;
    }

    wattron(subWindows.text, A_REVERSE);
    if (c >= 32 && c <= 126) {
        mvwprintw(subWindows.text, y, x, "%c", c);
    }
    else {
        mvwprintw(subWindows.text, y, x, " ");
    }
    wattroff(subWindows.text, A_REVERSE);

    wrefresh(subWindows.text);

    wattron(subWindows.hex, A_REVERSE);
    mvwprintw(subWindows.hex, y, x*3, foo ? "%02X" : "  ", c);
    wattroff(subWindows.hex, A_REVERSE);

    wrefresh(subWindows.hex);
}

template<typename T, typename R>
void Window::moveCursor(T y, T x, R current, T y_prev, T x_prev, R current_prev)
{
    if (buffer->bytes[current_prev] >= 32 && buffer->bytes[current_prev] <= 126) {
        mvwprintw(subWindows.text, y_prev, x_prev, "%c", buffer->bytes[current_prev]);
    }
    else {
        mvwprintw(subWindows.text, y_prev, x_prev, " ");
    }
    mvwprintw(subWindows.hex, y_prev, x_prev*3, "%02X", buffer->bytes[current_prev]);

    placeCursor();
}

void Window::replaceByte()
{
    curs_set(TRUE);
    echo();

    char buf[3] = "00";

    mvwprintw(subWindows.hex, y, x*3, "  ");
    wmove(subWindows.hex, y, x*3 + 1);

    buf[0] = wgetch(subWindows.hex);
    mvwprintw(subWindows.hex, y, x*3 + 1, "%c", buf[0]);

    wmove(subWindows.hex, y, x*3 + 1);
    buf[1] = wgetch(subWindows.hex);

    buffer->bytes[current] = std::stoi(buf, 0, 16);

    placeCursor();
    noecho();
    curs_set(FALSE);
}

void Window::save()
{
    buffer->save();
}

// vim: fen
