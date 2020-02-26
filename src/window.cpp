#include <string>
#include "window.hpp"

Window::Window(short height_, short width_, Buffer& buffer_) :
    height(height_), width(width_),
    cols((width-10)/4),
    currentByte(0),
    y(0), x(0),
    buffer(&buffer_)
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
    wrefresh(subWindows.statusline);
}

void Window::buf(Buffer& b)
{
    y = 0;
    x = 0;
    currentByte = 0;
    buffer = &b;
    fill();
}

void Window::redraw(short height_, short width_)
{
    applyToSubWindows(wclear);
    applyToSubWindows(wrefresh);

    if (COLS > 13) {  // on less than that, won't fit on screen
        height = height_;
        width  = width_;
        cols   = (width-10)/4;

        applyToSubWindows(delwin);
        genSubWindows();

        fill();
        updateStatusLine();

        y = currentByte / cols;
        x = currentByte % cols;

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
    // *_prev are for cleaning cursor background highlight
    short x_prev = x;
    short y_prev = y;
    size_t current_prev = currentByte;

    if (direction == Direction::down && currentByte + cols < buffer->size()) {
        currentByte += cols;
        y++;
    }
    else if (direction == Direction::up && currentByte - cols < buffer->size()) {
        currentByte -= cols;
        y--;
    }
    else if (direction == Direction::right && currentByte + 1 < buffer->size() && x < cols - 1) {
        currentByte++;
        x++;
    }
    else if (direction == Direction::left && currentByte > 0 && x > 0) {
        currentByte--;
        x--;
    }

    if (current_prev != currentByte) {
        moveCursor(y_prev, x_prev, current_prev);
        updateStatusLine();
    }
}

void Window::placeCursor()
{
    // Scrolling
    if (y >= height - 1) { // scrolling down
        buffer->print(*(this), currentByte/cols - height + 2);
        applyToSubWindows(wrefresh);
        y = height - 2;
    } else if (y < 0) { // scrolling up
        buffer->print(*(this), currentByte/cols + y + 1);
        applyToSubWindows(wrefresh);
        y = 0;
    }

    bool bufferEmpty = true;
    unsigned char c;
    if (buffer->size()) {
        c = buffer->bytes[currentByte];
        bufferEmpty = false;
    } else {
        c = 0; // buffer is empty, so let's set `c` to non-printable character
    }

    wattron(subWindows.hex, A_REVERSE);
    mvwprintw(subWindows.hex, y, x*3, bufferEmpty ? "  " : "%02X", c);
    wattroff(subWindows.hex, A_REVERSE);

    wattron(subWindows.text, A_REVERSE);
    if (c >= 32 && c <= 126) {
        mvwprintw(subWindows.text, y, x, "%c", c);
    }
    else {
        mvwprintw(subWindows.text, y, x, " ");
    }
    wattroff(subWindows.text, A_REVERSE);

    wrefresh(subWindows.hex);
    wrefresh(subWindows.text);

}

template<typename T, typename R>
void Window::moveCursor(T y_prev, T x_prev, R current_prev)
{
    // clear cursor background highlight
    if (buffer->bytes[current_prev] >= 32 && buffer->bytes[current_prev] <= 126) {
        mvwprintw(subWindows.text, y_prev, x_prev, "%c", buffer->bytes[current_prev]);
    }
    else {
        mvwprintw(subWindows.text, y_prev, x_prev, " ");
    }
    mvwprintw(subWindows.hex, y_prev, x_prev*3, "%02X", buffer->bytes[current_prev]);

    placeCursor(); // and place it on new position
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

    buffer->bytes[currentByte] = std::stoi(buf, 0, 16);

    placeCursor();
    noecho();
    curs_set(FALSE);
}

void Window::save()
{
    buffer->save();
}

// vim: fen
