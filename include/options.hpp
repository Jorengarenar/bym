#pragma once

#include <string>
#include <map>

class Options {
    std::map<std::string, std::string> opts;

public:
    Options();
    void set(std::string);
    void set(std::string, std::string);
    std::string get(std::string);
};
