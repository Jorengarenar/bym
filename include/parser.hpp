#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

#include <lua.hpp>

enum class Command {
    ECHO,
    MAP,
    NEWWIN,
    QUIT,
    REDRAW,
    SAVE,
    SAVEALL,
    SAVEALLQUIT,
    SAVEQUIT,
    SET,
    SETLOCAL,
};

/// Parser
///
/// Reads configs, executes commands etc.
class Parser {
    lua_State* L;
public:
    std::set<std::string> commandsKeys;
    Parser();
    ~Parser();

    bool operator () (std::string); ///< Parse string
    void config(); ///< Read config file
};
