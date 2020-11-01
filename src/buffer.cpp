#include "buffer.hpp"

#include <fstream>
#include <iterator>

#include <ncurses.h>

#include "window.hpp"
#include "util.hpp"
#include "editor.hpp"

Buffer::Buffer() : options(Editor().options), path(""), text("") {}

Buffer::Buffer(const char* p) : options(Editor().options), path(p)
{
    std::ifstream file(p, std::ios::binary);
    bytes = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
    file.close();
}

size_t Buffer::size()
{
    return bytes.size();
}

bool Buffer::empty()
{
    return !bytes.size();
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
