#pragma once

#include <ncurses.h>
#include <string>
#include <vector>

class Buffer {
    friend class Window;

  public:
    Buffer();
    Buffer(const char*);
    size_t size();

  private:
    std::string path;
    std::string text;
    std::vector<unsigned char> bytes;

    void print(WINDOW*, WINDOW*, WINDOW*, short, short);
    void save();
    void save(const char*);
};
