#include "parser.hpp"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>

#include "editor.hpp"
#include "util.hpp"

const std::map<std::string, Command> commands{
    { "echo",     Command::ECHO         },
    { "map",      Command::MAP          },
    { "newwin",   Command::NEWWIN       },
    { "q",        Command::QUIT         },
    { "quit",     Command::QUIT         },
    { "redraw",   Command::REDRAW       },
    { "set",      Command::SET          },
    { "setlocal", Command::SETLOCAL     },
    { "w",        Command::SAVE         },
    { "wa",       Command::SAVEALL      },
    { "wq",       Command::SAVEQUIT     },
    { "wqa",      Command::SAVEALLQUIT  },
};

bool Parser::operator ()(std::string line)
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
            case Command::QUIT:
                return false;
                break;
            case Command::SAVE:
                Editor().cw->save();
                break;
            case Command::SAVEQUIT:
                Editor().cw->save();
                return false;
                break;

            case Command::REDRAW:
                Editor().cw->redraw();
                break;
            case Command::MAP:
                break;

            case Command::ECHO:
            case Command::SET:
            case Command::SETLOCAL:
                {
                    std::string temp;
                    buf >> temp;
                    switch (c->second) {
                        case Command::ECHO:
                            Editor().cli.echo(temp);
                            break;
                        case Command::SET:
                            Editor().setOption(temp);
                            break;
                        case Command::SETLOCAL:
                            Editor().cw->buffer->options.set(temp);
                            break;
                        default:
                            break;
                    }
                }
                break;

            default:
                break;
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
