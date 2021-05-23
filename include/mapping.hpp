#pragma once

#include <map>
#include <vector>

#include <jcbcpp/trie.hpp>
#include <ncurses.h>

#include "util.hpp"

/// Defined secular actions
enum class Action {
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    RESIZE,
    QUIT,
    SEARCH,
    SAVEQUIT,
    CMD,
    REDRAW,
    REPLACE,
    FIRSTCOL,
    LASTCOL,
    NEXTBYTE,
    FIRSTBYTE,
    LASTBYTE,
    INCREMENT,
    DECREMENT,
};

class Mappings {
    using returnType = std::vector<Action>;
    using foo = std::map<std::vector<int>, returnType>;
    const foo defaults;
    jcbcpp::Trie<int> bar;
public:
    Mappings();
    returnType operator ()(int);
};
