#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

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
public:
    std::set<std::string> commandsKeys;
    Parser();

    bool operator () (std::string); ///< Parse
    void config(); ///< Read config file
};
