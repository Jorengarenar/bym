#include "options.hpp"

Options::Options() :
    opts{
         { "cols",  "0" },
         { "blank", "." },
         { "upper", "1" },
        }
{}

void Options::set(std::string opt)
{
    std::string o = opt.substr(0, opt.find("="));
    set(o, opt.substr(opt.find("=") + 1, opt.length()));
}

void Options::set(std::string o, std::string val)
{
    auto opt = opts.find(o);
    if (opt != opts.end()) {
        opt->second = val;
    }
}

std::string Options::get(std::string o)
{
    auto opt = opts.find(o);
    if (opt != opts.end()) {
        return opt->second;
    }

    return "";
}
