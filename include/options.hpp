#pragma once

#include <string>
#include <map>

class Options {
    std::map<std::string, std::string> opts;

public:
    Options();
    void set(std::string); //< for CLI commands in form of `set opt=...`
    void set(std::string, std::string);
    std::string get(std::string);

    friend class Parser;
};
