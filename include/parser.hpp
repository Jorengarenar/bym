#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <set>

class Editor;

enum class Command {
    quit, save, savequit
};

class Parser {
    Editor& editor;
public:
    std::set<std::string> commandsKeys;
    Parser(Editor&);

    bool operator()(std::string);
};
