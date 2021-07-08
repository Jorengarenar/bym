#include "window.hpp"

#include <cctype>
#include <string>

#include "editor.hpp"
#include "util.hpp"

Window::Window(Buffer& buffer_) :
    currentByte(0), prevByte(0),
    startline(0),
    buffer(&buffer_),
    y(0), x(0),
    opts(*this)
{
    genSubWindows();
    print();
    placeCursor();
}

Window::~Window()
{
    delSubWindows();
}

inline short Window::height() const
{
    return LINES - 3; // <lines> - <cli> - <tabline> - <statusline>
}

inline short Window::width() const
{
    return COLS;
}

void Window::genSubWindows()
{
    delSubWindows();
    unsigned short c = opts.cols();
    auto h = height();
    subWindows = { // be wary of order in declaration of Window class!
        newwin(h, 10,  1, 0),        // numbers
        newwin(h, c*3, 1, 10),       // hex
        newwin(h, c+5, 1, c*3 + 10)  // text
    };
    refresh();
}

void Window::delSubWindows()
{
    applyToSubWindows(delwin);
    subWindows = { nullptr, nullptr, nullptr };
}

void Window::buf(Buffer& b)
{
    startline = y = x = 0;
    currentByte = 0;
    buffer = &b;
    print();
}

void Window::redraw()
{
    applyToSubWindows(wclear);

    if (COLS > 13) {  // on less than that, won't fit on screen
        genSubWindows();
        print();
        placeCursor();
    }
}

void Window::applyToSubWindows(std::function<void (WINDOW*)> f)
{
    if (subWindows.hex)     { f(subWindows.hex);     }
    if (subWindows.text)    { f(subWindows.text);    }
    if (subWindows.numbers) { f(subWindows.numbers); }
}

void Window::printByte(const unsigned char b, std::size_t x, std::size_t y, int attr)
{
    auto p = [&](WINDOW* w, int X, const char* fmt, unsigned char C) {
             wattron(w, attr);
             mvwprintw(w, y, X, fmt, C);
             wattroff(w, attr);
             wrefresh(w);
         };

    p(subWindows.hex, x*3, opts.hexFmt(), b);
    p(subWindows.text, x, "%c", isprint(b) ? b : opts.blank());
}

void Window::print()
{
    const auto cols = opts.cols();

    applyToSubWindows([](WINDOW* w) { wmove(w, 0, 0); });

    unsigned short c = 0; // current column of bytes
    unsigned short l = 0; // current line
    for (std::size_t i = startline*cols; l < height() && i < buffer->size(); ++i) {
        if (c == 0) {
            wprintw(subWindows.numbers, "%08X:\n", i);
        }

        printByte(buffer->bytes[i], c, l);

        ++c;
        if (c == cols) {
            c = 0;
            ++l;
        }
    }

    applyToSubWindows(wclrtoeol);
    applyToSubWindows(wrefresh);
}

void Window::gotoByte(std::size_t b)
{
    if (b < buffer->size()) {
        prevByte = currentByte;
        currentByte = b;
        placeCursor();
    }
}

void Window::mvCursor(Direction d)
{
    auto s = buffer->size();
    auto c = opts.cols();
    auto x = currentByte % c;

    prevByte = currentByte;

    if (d == Direction::DOWN && currentByte + c < s) {
        currentByte += c;
    }
    else if (d == Direction::UP && currentByte - c < s) {
        currentByte -= c;
    }
    else if (d == Direction::RIGHT && currentByte + 1 < s && x + 1 < c) {
        ++currentByte;
    }
    else if (d == Direction::LEFT && currentByte > 0 && x > 0) {
        --currentByte;
    }

    placeCursor();
}

void Window::mvCursor(std::size_t X, std::size_t Y)
{
    auto c = opts.cols();
    auto maxY = buffer->size() / c;
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
    auto& b = *buffer;
    auto C = opts.cols();

    if (prevByte != currentByte) { // Clear previous cursor background highlight
        auto x_prev = prevByte % C;
        auto y_prev = prevByte / C - startline;
        printByte(b[prevByte], x_prev, y_prev);
    }

    auto curLine = currentByte / C;
    auto maxLine = b.size() / C;

    auto off = opts.scrolloff();

    // Scrolling
    if (curLine >= startline + height() - off) { // scrolling down
        startline = curLine - height() + off + 1;
        if (curLine >= maxLine - off) {
            startline -= (off - (maxLine - curLine));
        }
        print();
    }
    else if (curLine < startline + off) { // scrolling up
        startline = curLine - off;
        if (curLine <= off) {
            startline += (off - curLine);
        }
        print();
    }

    x = currentByte % C;
    y = curLine - startline;

    unsigned char c = 0;
    if (!buffer->empty()) {
        c = b[currentByte];
    }

    printByte(c, x, y, A_REVERSE);

    Editor().updateStatusline();
}

bool Window::inputByte(char* buf)
{
    mvwprintw(subWindows.hex, y, x*3, "  ");
    EnableCursor cur;

    for (int i = 0; i < 2; ) { // TODO: value 2 is temporary, it will be mutable
        wmove(subWindows.hex, y, x*3 + i);
        int b = wgetch(subWindows.hex);

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
        (*buffer)[currentByte] = b;
    } // No `else`, bc `placeCursor()` already handles reprinting correct byte

    placeCursor();

    return 0;
}

void Window::addToByte(unsigned char x)
{
    (*buffer)[currentByte] += x;
    placeCursor();
}

void Window::save()
{
    buffer->save();
}

// --/--/-- OPTIONS --/--/--/--

Window::Opts::Opts(Window& w_) : w(w_) {}

unsigned short Window::Opts::cols() const
{
    auto c = std::stoi(w.buffer->getOption("cols"));
    auto C = (w.width()-10)/4;
    if (c > 0 && c <= C) {
        return c;
    }
    return C;
}

char Window::Opts::blank() const
{
    return w.buffer->getOption("blank").at(0);
}

const char* Window::Opts::hexFmt() const
{
    return w.buffer->options.get("upper") == "1" ? "%02X" : "%02x";
}

unsigned short Window::Opts::scrolloff() const
{
    return std::stoi(w.buffer->getOption("scrolloff"));
}
