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

public:
    using byteType = unsigned char;

    Buffer();
    Buffer(const char* path); ///< Constructor with path to file

    size_t size(); ///< Size of file
    bool empty();  ///< Checks if file is of size 0

    Options options;
    std::string getOption(std::string);

    byteType operator  [](std::size_t n) const;
    byteType& operator [](std::size_t n);

    void eraseByte(std::size_t);

    std::size_t findByte(
        std::function<bool(byteType)> predicate,
        std::size_t start = 0, std::size_t endOffset = 0
    ) const;

    std::size_t findBlock(
        std::function<bool(byteType)> predicate,
        std::size_t start = 0, std::size_t endOffset = 0
    ) const;

    std::string path; ///< Path to opened file
private:
    std::vector<byteType> bytes; ///< Contains bytes
    void save(std::string = ""); ///< Saves edits to file
};
