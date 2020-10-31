#include "parser.hpp"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>

#include "editor.hpp"
#include "util.hpp"

const std::map<std::string, Command> commands{
    { "echo",   Command::echo },
    { "map",    Command::map },
    { "q",      Command::quit },
    { "quit",   Command::quit },
    { "redraw", Command::redraw },
    { "set",    Command::set },
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
            case Command::echo: {
                std::string msg;
                buf >> msg;
                Editor().cli.echo(msg);
            } break;
            case Command::set: {
                std::string opt;
                buf >> opt;
                Editor().set(opt);
            } break;
        }

    }
    return true;
}

void Parser::config()
{
    std::string xdg_config_home = getEnvVar("XDG_CONFIG_HOME");
    if (xdg_config_home.empty()) {
        std::string home = getEnvVar("HOME");
        if (!home.empty()) {
            xdg_config_home = home + "/.config";
        }
    }

    std::ifstream conf{ xdg_config_home + "/bym/bymrc" };

    if (conf.is_open()) {
        std::string buf;
        while (getline(conf, buf)) {
            (*this)(buf);
        }
        conf.close();
    }
}

Parser::Parser()
{
    std::transform(::commands.begin(), ::commands.end(),
                   std::inserter(commandsKeys, commandsKeys.begin()),
                   [](const auto& p) { return p.first; });
}
