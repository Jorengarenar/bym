#include "parser.hpp"

int parse(std::string buf)
{
    return buf != "q";
    if (buf == "q") {
        return 0;
    }
    else if (buf == "w") {
        return 1;
    }
    return 3;
}
