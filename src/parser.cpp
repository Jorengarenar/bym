#include "parser.hpp"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>

#include "editor.hpp"

const std::map<std::string, Command> commands{
    { "q",      Command::quit },
    { "quit",   Command::quit },
    { "w",      Command::save },
    { "wq",     Command::savequit },
};

bool Parser::operator ()(std::string line)
{
    std::stringstream buf{ line };
    std::string a;

    while (buf >> a) {

        if (a == "|") {
            continue;
        }

        auto c = commands.find(a);

        if (c == commands.end()) {
            Editor().cli.error(a + " : No such command!");
            return true;
        }

        switch (c->second) {
            case Command::quit:
                return false;
                break;
            case Command::save:
                Editor().cw->save();
                break;
            case Command::savequit:
                Editor().cw->save();
                return false;
                break;
        }

    }
    return true;
}

Parser::Parser()
{
    std::transform(::commands.begin(), ::commands.end(),
                   std::inserter(commandsKeys, commandsKeys.begin()),
                   [](const auto& p) { return p.first; });
}
