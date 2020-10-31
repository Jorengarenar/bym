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
    Buffer(const char*); ///< Constructor with path to file

    size_t size(); ///< Size of file
    bool empty();  ///< Checks if file is of size 0

    int getOption(std::string);

private:
    std::string path; ///< Path to opened file
    std::string text; ///< Text representation of bytes
    std::vector<unsigned char> bytes; ///< Contains bytes
    void save(std::string = ""); ///< Saves edits to file
};
