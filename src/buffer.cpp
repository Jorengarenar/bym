#include "buffer.hpp"

#include <fstream>
#include <iterator>

#include <ncurses.h>

#include "window.hpp"
#include "util.hpp"

Buffer::Buffer() : path(""), text("") {}

Buffer::Buffer(const char* p) : path(p)
{
    std::ifstream file(p, std::ios::binary);
    bytes = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
    file.close();
}

size_t Buffer::size()
{
    return bytes.size();
}

void Buffer::print(Window& win, short startLine)
{
    std::string str = ""; // representation of bytes in line
    int y = 0; // current line
    int x = 0; // current column of bytes

    int maxY = win.height - 1 + startLine;

    win.applyToSubWindows([](WINDOW* w) { wmove(w, 0, 0); });

    auto& subWins = win.subWindows;

    if (bytes.size()) {
        for (size_t i = 0; i < bytes.size() && y < maxY; ++i) {
            if (y >= startLine) {
                if (x == 0) {
                    wprintw(subWins.numbers, "%08X:\n", i);
                }

                wprintw(subWins.hex, "%02X ", bytes[i]);

                str += toPrintable(bytes[i]);

                ++x;
                if (x == win.cols) {
                    x = 0;
                    wprintw(subWins.text, "%s\n", str.c_str());
                    str = "";
                    ++y;
                }
            }
            else {
                ++x;
                if (x == win.cols) {
                    x = 0;
                    ++y;
                }
            }
        }
    }
    else {
        wprintw(win.subWindows.numbers, "%08X: ", 0);
    }

    // Print any remaining bytes if main loop ended because of end of buffer
    if (y < win.height) {
        while (x < win.cols) {
            wprintw(subWins.hex, "   ");
            ++x;
        }
        wprintw(subWins.text, "%s\n", str.c_str());
    }
}

void Buffer::save(std::string p)
{
    if (p.empty()) {
        p = path;
    }
    std::ofstream output(p, std::ios::binary);
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(output));
    output.close();
}

// vim: fen
