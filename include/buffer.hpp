#pragma once

#include <string>
#include <vector>

#include <ncurses.h>

class Window;

/// Buffer
///
/// A buffer is a file loaded into memory for editing
class Buffer {
    friend class Window;

  public:
    Buffer();
    Buffer(const char*);
    size_t size();
    bool empty();

  private:
    std::string path; // path to opened file
    std::string text;
    std::vector<unsigned char> bytes;

    void save(std::string = "");
};
