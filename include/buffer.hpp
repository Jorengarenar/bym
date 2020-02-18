#pragma once

#include <ncurses.h>
#include <string>
#include <vector>

class Window;

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

    void print(Window&, short, short);
    void save();
    void save(const char*);
};
