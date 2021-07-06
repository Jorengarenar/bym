#pragma once

#include <algorithm>
#include <string>

/// Code of combination of CTRL and other key
#define CTRL(x)  ((x) & 0x1f)

const char ESC = 27;

/// Enables terminal cursor
struct EnableCursor {
    EnableCursor();
    ~EnableCursor();
};

std::string getLastWord(const std::string&);

/// Checks if string is prefix to another string
bool isPrefix(const std::string& prefix, const std::string& data);

/// Reads environmental variables
std::string getEnvVar(const std::string& var);
