#include "parser.hpp"

#include <sstream>
#include <iterator>

#include "editor.hpp"

const std::map<std::string, Command> commands {
    { "q",    Command::quit },
    { "quit", Command::quit },
    { "w",    Command::save },
    { "wq",   Command::savequit },
};

bool Parser::operator()(std::string line)
{
    std::stringstream buf{ line };
    std::string a;

    while (buf >> a) {

        if (a == "|") {
            continue;
        }

        auto c = commands.find(a);

        if (c == commands.end()) {
            editor.cmd.error(a + " : No such command!");
            return true;
        }

        switch (c->second) {
            case Command::quit:
                return false;
                break;
            case Command::save:
                editor.cw->save();
                break;
            case Command::savequit:
                editor.cw->save();
                return false;
                break;
        }

    }
    return true;
}

Parser::Parser(Editor& e) : editor(e)
{
    std::transform(::commands.begin(), ::commands.end(),
                   std::inserter(commandsKeys, commandsKeys.begin()),
                   [](const auto& p) { return p.first; });
}
