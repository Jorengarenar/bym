#include <fstream>
#include <iterator>
#include <ncurses.h>

#include "buffer.hpp"

Buffer::Buffer()
{
    path = "";
    text = "";
}

Buffer::Buffer(const char* p)
{
    path = p;
    std::ifstream file(p, std::ios::binary);
    bytes = std::vector<unsigned char> (std::istreambuf_iterator<char>(file), {});

    for (unsigned char b: bytes) {
        text += b;
    }

    file.close();
}

size_t Buffer::size()
{
    return bytes.size();
}

void Buffer::print(WINDOW* const win, short height, const short cols)
{
    std::string str = "";
    int x = 0;
    int y = 0;
    wclear(win);
    wmove(win, 0,0);
    if (bytes.size()) {
        for (int i = 0; i < bytes.size() && y < height; i++) {
            if (x == 0) {
                wprintw(win, "%08X: ", i);
            }
            wprintw(win, "%02X ", bytes[i]);

            if (bytes[i] >= 32 && bytes[i] <= 126) {
                str += bytes[i];
            }
            else {
                str += ' ';
            }

            if (++x == cols) {
                x = 0;
                wprintw(win, "| %s\n", str.c_str());
                str = "";
                y++;
            }
        }
    }
    else {
        wprintw(win, "%08X: ", 0);
    }

    if (y < height) {
        while (x < cols) {
            wprintw(win, "   ");
            x++;
        }
        wprintw(win, "| %s\n", str.c_str());
    }
}

void Buffer::save()
{
    std::ofstream output(path, std::ios::binary);
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(output));
    output.close();
}

void Buffer::save(const char* p)
{
    std::ofstream output(p, std::ios::binary);
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(output));
    output.close();
}

// vim: fen
