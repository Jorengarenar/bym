#include "buffer.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

#include <ncurses.h>

#include "window.hpp"
#include "util.hpp"
#include "editor.hpp"

Buffer::Buffer() : options(Editor().options), path("") {}

Buffer::Buffer(const char* p) : options(Editor().options), path(p)
{
    std::ifstream file(p, std::ios::binary | std::ios::ate);
    std::size_t size = file.tellg(); // Determine the file length
    file.seekg(0, std::ios::beg);
    bytes.resize(size);
    file.read(reinterpret_cast<char*>(bytes.data()), size);
    file.close();
}

size_t Buffer::size()
{
    return bytes.size();
}

bool Buffer::empty()
{
    return bytes.empty();
}

void Buffer::save(std::string p)
{
    if (p.empty()) {
        p = path;
    }
    std::ofstream output(p, std::ios::binary);
    std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(output));
    output.close();
}

std::string Buffer::getOption(std::string o)
{
    return options.get(o);
}

Buffer::byteType Buffer::operator [](std::size_t n) const
{
    return bytes[n];
}

Buffer::byteType& Buffer::operator [](std::size_t n)
{
    return bytes[n];
}

void Buffer::eraseByte(std::size_t i)
{
    bytes.erase(bytes.begin() + i);
}

std::size_t Buffer::findByte(
    std::function<bool(byteType)> predicate,
    std::size_t start, std::size_t endOffset
) const
{
    const auto&& begin = bytes.begin() + start + 1;
    const auto&& end   = bytes.end()   - endOffset;
    auto it = std::find_if(begin, end, predicate);
    if (it != end) {
        return std::distance(bytes.begin(), it);
    }
    return start;
}

std::size_t Buffer::findBlock(
    std::function<bool(byteType)> predicate,
    std::size_t start, std::size_t endOffset
) const
{
    while (predicate(bytes[start])) { ++start; }
    return findByte(predicate, start, endOffset);
}
