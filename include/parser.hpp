#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

enum class Command {
    echo,
    map,
    newwin,
    quit,
    redraw,
    save,
    saveall,
    saveallquit,
    savequit,
    set,
    setlocal,
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
