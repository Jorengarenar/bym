#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

enum class Command {
    quit, save, savequit
};

/// Parser
///
/// Reads configs, executes commands etc.
class Parser {
public:
    std::set<std::string> commandsKeys;
    Parser();

    bool operator ()(std::string); ///< Parse
};
