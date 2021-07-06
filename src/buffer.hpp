#pragma once

#include <string>
#include <vector>

#include <ncurses.h>

#include "options.hpp"

class Window;

/// Buffer
///
/// A buffer is a file loaded into memory for editing
class Buffer {
    friend class Window;

public:
    Buffer();
    Buffer(const char* path); ///< Constructor with path to file

    size_t size(); ///< Size of file
    bool empty();  ///< Checks if file is of size 0

    Options options;
    std::string getOption(std::string);

    unsigned char  operator [](std::size_t n) const;
    unsigned char& operator [](std::size_t n);

private:
    std::string path; ///< Path to opened file
    std::vector<unsigned char> bytes; ///< Contains bytes
    void save(std::string = ""); ///< Saves edits to file
};
