#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

class Editor;

enum class Command {
    quit, save, savequit
};

/// Parser
///
/// Reads configs, executes commands etc.
class Parser {
    Editor& editor; ///< Parent of this object
public:
    std::set<std::string> commandsKeys;
    Parser(Editor&);

    bool operator ()(std::string); ///< Parse
};
