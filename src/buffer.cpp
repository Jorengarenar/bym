#include <fstream>
#include <iterator>
#include <ncurses.h>

#include "buffer.hpp"
#include "window.hpp"

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

    win.applyToSubWindows([](WINDOW* w) { wmove(w, 0, 0); });

    if (bytes.size()) {
        for (int i = 0; i < bytes.size() && y < win.height - 1 + startLine; i++) {
            if (y >= startLine) {
                if (x == 0) {
                    wprintw(win.subWindows.numbers, "%08X:\n", i);
                }

                wprintw(win.subWindows.hex, "%02X ", bytes[i]);

                if (bytes[i] >= 32 && bytes[i] <= 126) {
                    str += bytes[i];
                }
                else {
                    str += ' ';
                }

                if (++x == win.cols) {
                    x = 0;
                    wprintw(win.subWindows.text, "%s\n", str.c_str());
                    str = "";
                    y++;
                }
            }
            else if (++x == win.cols) {
                x = 0;
                y++;
            }
        }
    }
    else {
        wprintw(win.subWindows.numbers, "%08X: ", 0);
    }

    // Print any remaining bytes if main loop ended because of end of buffer
    if (y < win.height) {
        while (x < win.cols) {
            wprintw(win.subWindows.hex, "   ");
            x++;
        }
        wprintw(win.subWindows.text, "%s\n", str.c_str());
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
