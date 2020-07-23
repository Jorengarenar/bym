#include "util.hpp"

#include <cctype>
#include <cstdlib>

#include <ncurses.h>

EnableCursor::EnableCursor()
{
    curs_set(TRUE);
}

EnableCursor::~EnableCursor()
{
    curs_set(FALSE);
}

char toPrintable(int x)
{
    if (isprint(x)) {
        return x;
    }
    else {
        return ' ';
    }
}

std::string getLastWord(const std::string& s)
{
    return s.substr(s.find_last_of(' ') + 1);
}

bool isPrefix(const std::string& prefix, const std::string& data)
{
    if (prefix.length() > data.length()) {
        return false;
    }

    auto mismatch = std::mismatch(data.begin(),   data.end(),
                                  prefix.begin(), prefix.end()
                                 ).second;

    return mismatch == prefix.end();
}
