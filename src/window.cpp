#include "window.hpp"

#include <cctype>
#include <string>

#include "util.hpp"

Window::Window(int height_, int width_, Buffer& buffer_) :
    height(height_), width(width_),
    currentByte(0),
    prevByte(0),
    startline(0),
    buffer(&buffer_),
    y(0), x(0),
    opts(*this)
{
    genSubWindows();
    print();
    updateStatusLine();
    placeCursor();
}

Window::~Window()
{
    applyToSubWindows(delwin);
}

void Window::genSubWindows()
{
    applyToSubWindows(delwin);
    unsigned short c = opts.cols();
    refresh();
    subWindows = { // be wary of order in declaration of Window class!
        newwin(height, 10, 0, 0),         // numbers
        newwin(height, c*3, 0, 10),       // hex
        newwin(height, c+5, 0, c*3 + 10), // text
        nullptr                           // statusline
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
    wprintw(subWindows.statusline, "%s %zu", buffer->path.c_str(), buffer->size()); // TODO

    wrefresh(subWindows.statusline);
}

void Window::buf(Buffer& b)
{
    startline = y = x = 0;
    currentByte = 0;
    buffer = &b;
    print();
}

void Window::redraw(short height_, short width_)
{
    applyToSubWindows(wclear);
    applyToSubWindows(wrefresh);

    if (COLS > 13) {  // on less than that, won't fit on screen
        height = height_;
        width  = width_;

        genSubWindows();

        placeCursor();
    }
}

void Window::applyToSubWindows(std::function<void (WINDOW*)> f)
{
    if (subWindows.hex)        { f(subWindows.hex);        }
    if (subWindows.text)       { f(subWindows.text);       }
    if (subWindows.numbers)    { f(subWindows.numbers);    }
    if (subWindows.statusline) { f(subWindows.statusline); }
}

void Window::print()
{
    auto c = opts.cols();

    std::string str = ""; // representation of bytes in line
    int y = startline; // current line
    int x = 0; // current column of bytes

    int maxY = height - 1 + startline;

    applyToSubWindows([](WINDOW* w) { wmove(w, 0, 0); });

    auto& bytes = buffer->bytes;
    auto& subWins = subWindows;

    if (bytes.size()) {
        for (std::size_t i = y*c; i < bytes.size() && y < maxY; ++i) {
            if (x == 0) {
                wprintw(subWins.numbers, "%08X:\n", i);
            }

            wprintw(subWins.hex, "%02X ", bytes[i]);

            str += toPrintable(bytes[i], opts.blank());

            ++x;
            if (x == c) {
                x = 0;
                wprintw(subWins.text, "%s\n", str.c_str());
                str = "";
                ++y;
            }
        }
    }
    else {
        wprintw(subWindows.numbers, "%08X: ", 0);
    }

    // Print any remaining bytes if main loop ended because of end of buffer
    if (y < maxY) {
        while (x < c) {
            wprintw(subWindows.hex, "   ");
            ++x;
        }
        wprintw(subWindows.text, "%s\n", str.c_str());
    }

    applyToSubWindows(wrefresh);
}

void Window::mvCursor(Direction d)
{
    auto s = buffer->size();
    auto c = opts.cols();
    auto x = currentByte % c;

    prevByte = currentByte;

    if (d == Direction::down && currentByte + c < s) {
        currentByte += c;
    }
    else if (d == Direction::up && currentByte - c < s) {
        currentByte -= c;
    }
    else if (d == Direction::right && currentByte + 1 < s && x < c - 1) {
        ++currentByte;
    }
    else if (d == Direction::left && currentByte > 0 && x > 0) {
        --currentByte;
    }

    placeCursor();
}

void Window::mvCursor(std::size_t X, std::size_t Y)
{
    auto c = opts.cols();
    auto maxY = (buffer->size()-1) / c;
    if (Y >= maxY) {
        Y = maxY;
        auto maxX = buffer->size() % c;
        if (X > maxX) {
            X = maxX;
        }
    }
    else if (X >= c) {
        X = c-1;
    }

    prevByte = currentByte;
    currentByte = Y * c + X;

    placeCursor();
}

void Window::placeCursor()
{
    auto C = opts.cols();

    auto x_prev = prevByte % C;
    auto y_prev = prevByte / C - startline;

    if (prevByte != currentByte) {
        // Clear previous cursor background highlight
        mvwprintw(subWindows.text, y_prev, x_prev, "%c", toPrintable(buffer->bytes[prevByte], opts.blank()));
        mvwprintw(subWindows.hex, y_prev, x_prev*3, "%02X", buffer->bytes[prevByte]);
    }

    // Scrolling
    if (currentByte/C >= startline + height - 1) { // scrolling down
        ++startline;
        print();
    }
    else if (currentByte/C < startline) { // scrolling up
        --startline;
        print();
    }

    x = currentByte % C;
    y = currentByte / C - startline;

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
    printCursor(subWindows.text, x, "%c", toPrintable(c, opts.blank()));

    updateStatusLine();
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
        mvwprintw(subWindows.text, y, x, "%c", toPrintable(b, '.'));
    } // No `else`, bc `placeCursor()` already handles printing correct byte

    placeCursor();

    return 0;
}

void Window::save()
{
    buffer->save();
}

// --/--/-- OPTIONS --/--/--/--

Window::Opts::Opts(Window& w_) : w(w_)
{}

unsigned short Window::Opts::cols() const
{
    auto c = std::stoi(w.buffer->getOption("cols"));
    auto C = (w.width-10)/4;
    if (c > 0 && c <= C) {
        return c;
    }
    return C;
}

char Window::Opts::blank() const
{
    return w.buffer->getOption("blank").at(0);
}
