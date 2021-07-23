#include "options.hpp"

Options::Options() :
    opts{
         { "cols",      "32" },
         { "blank",     "."  },
         { "upper",     "1"  },
         { "scrolloff", "5"  },
        }
{}

void Options::set(std::string opt)
{
    std::string o = opt.substr(0, opt.find("="));
    set(o, opt.substr(opt.find("=") + 1, opt.length()));
}

void Options::set(const std::string& o, const std::string& val)
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
