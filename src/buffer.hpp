#pragma once

#include <string>
#include <vector>
#include <functional>

#include <ncurses.h>

#include "options.hpp"

class Window;

/// Buffer
///
/// A buffer is a file loaded into memory for editing
class Buffer {
    friend class Window;
    using byteType = unsigned char;

public:
    Buffer();
    Buffer(const char* path); ///< Constructor with path to file

    size_t size(); ///< Size of file
    bool empty();  ///< Checks if file is of size 0

    Options options;
    std::string getOption(std::string);

    byteType  operator [](std::size_t n) const;
    byteType& operator [](std::size_t n);

    std::size_t findByte(std::size_t startOffset, std::size_t endOffset,
                         std::size_t fallback,
                         std::function<bool(byteType)> cmp) const;

private:
    std::string path; ///< Path to opened file
    std::vector<byteType> bytes; ///< Contains bytes
    void save(std::string = ""); ///< Saves edits to file
};
