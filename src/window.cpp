#include "window.hpp"

#include <cctype>
#include <string>

#include "util.hpp"

Window::Window(int height_, int width_, Buffer& buffer_) :
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
    refresh();
    subWindows = { // be wary of order in declaration of Window class!
        newwin(height, 10, 0, 0),               // numbers
        newwin(height, cols*3, 0, 10),          // hex
        newwin(height, cols+5, 0, cols*3 + 10), // text
        nullptr                                 // statusline
    };
    updateStatusLine();
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
    unsigned short x_prev = x;
    unsigned short y_prev = y;
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
    }
    else if (y < 0) {      // scrolling up
        buffer->print(*(this), currentByte/cols + y + 1);
        applyToSubWindows(wrefresh);
        y = 0;
    }

    unsigned char c = 0;
    if (!buffer->empty()) {
        c = buffer->bytes[currentByte];
    }

    auto printCursor = [&](WINDOW* w, int X, const char* fmt, unsigned char C) {
        wattron(w, A_REVERSE);
        mvwprintw(w, y, X, fmt, C);
        wattroff(w, A_REVERSE);

        wrefresh(w);
    };

    printCursor(subWindows.hex, x*3, buffer->empty() ? "  " : "%02X", c);
    printCursor(subWindows.text, x, "%c", toPrintable(c));
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

bool Window::inputByte(char* buf)
{
    mvwprintw(subWindows.hex, y, x*3, "  ");

    EnableCursor cur;

    int b;
    for (int i = 0; i < 2; ) { // value 2 is temporary, it will be mutable
        wmove(subWindows.hex, y, x*3 + i);
        b = wgetch(subWindows.hex);

        switch (b) {
            case ::ESC:
            case CTRL('c'):
                return false;

            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (i) {
                    mvwprintw(subWindows.hex, y, x*3, " ");
                    --i;
                }
                break;

            default:
                if (isxdigit(b)) {
                    wprintw(subWindows.hex, "%c", b);
                    buf[i] = b;
                    ++i;
                }
        }
    }

    return true;
}

int Window::replaceByte()
{
    if (buffer->empty()) {
        return 1;
    }

    char buf[3] = "00";

    if (inputByte(buf)) {
        unsigned char b = std::stoi(buf, 0, 16);
        buffer->bytes[currentByte] = b;
        mvwprintw(subWindows.text, y, x, "%c", toPrintable(b));
    } // No `else`, bc `placeCursor()` already handles printing correct byte

    placeCursor();

    return 0;
}

void Window::save()
{
    buffer->save();
}
