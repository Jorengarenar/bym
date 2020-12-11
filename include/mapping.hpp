#pragma once

#include <map>
#include <vector>

#include <ncurses.h>

enum class Action {
    NONE, LEFT, RIGHT, UP, DOWN, RESIZE, CMD, REDRAW, REPLACE, FIRSTCOL
};

class Mappings {
    using returnType = std::vector<Action>;
    using foo = std::map<std::vector<int>, returnType>;
    const foo basics;
    const foo compounds;
public:
    Mappings();
    returnType operator ()(int);
};
