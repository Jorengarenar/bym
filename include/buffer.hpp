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
    std::string path; // path to opened file
    std::string text;
    std::vector<unsigned char> bytes;

    void print(Window&, short = 0); // called from Window::fill()

    void save(std::string = "");
};
