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

char toPrintable(int x, char blank)
{
    if (isprint(x)) {
        return x;
    }
    else {
        return blank;
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

std::string getEnvVar(const std::string& var)
{
    char* val = getenv(var.c_str());
    return val == NULL ? "" : val;
}
