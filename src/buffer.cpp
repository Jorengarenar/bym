#include <fstream>
#include <iterator>
#include <ncurses.h>

#include "buffer.hpp"

Buffer::Buffer(char* path)
{
    std::ifstream file(path, std::ios::binary);
    bytes = std::vector<unsigned char> (std::istreambuf_iterator<char>(file), {});
    file.close();
}

void Buffer::print()
{
    const int cols = 32;
    int i = 0;
    for (unsigned char b: bytes) {
        printw("%02X ", b);
        i++;
        if (i == cols) {
            i = 0;
            printw("\n");
        }
    }
}
