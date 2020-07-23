#pragma once

#include <algorithm>
#include <string>

#define CTRL(x)  ((x) & 0x1f)

const char ESC = 27;

struct InitCurses {
    InitCurses();
    ~InitCurses();
};

struct EnableCursor {
    EnableCursor();
    ~EnableCursor();
};

char toPrintable(int);

std::string getLastWord(const std::string&);

bool isPrefix(const std::string& prefix, const std::string& data);
