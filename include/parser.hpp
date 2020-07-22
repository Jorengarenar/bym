#pragma once

#include <string>

#include "editor.hpp"

enum class Command {
    nothing, quit, save, savequit
};

bool parse(Editor&, std::string);
