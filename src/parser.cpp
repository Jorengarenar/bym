#include "parser.hpp"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>

#include "editor.hpp"
#include "util.hpp"

const std::map<std::string, Command> commands{
    { "echo",     Command::echo         },
    { "map",      Command::map          },
    { "newwin",   Command::newwin       },
    { "q",        Command::quit         },
    { "quit",     Command::quit         },
    { "redraw",   Command::redraw       },
    { "set",      Command::set          },
    { "setlocal", Command::setlocal     },
    { "w",        Command::save         },
    { "wa",       Command::saveall      },
    { "wq",       Command::savequit     },
    { "wqa",      Command::saveallquit  },
};

bool Parser::operator () (std::string line)
{
    std::stringstream buf{ line };
    std::string a;

    while (buf >> a) {

        if (a == "|") {
            continue;
        }

        if (a == "#") {
            break;
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

            case Command::redraw:
                Editor().cw->redraw();
                break;
            case Command::map:
                break;

            case Command::echo:
            case Command::set:
            case Command::setlocal: {
                    std::string temp;
                    buf >> temp;
                    switch (c->second) {
                        case Command::echo:
                            Editor().cli.echo(temp);
                            break;
                        case Command::set:
                            Editor().setOption(temp);
                            break;
                        case Command::setlocal:
                            Editor().cw->buffer->options.set(temp);
                            break;
                    }
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
