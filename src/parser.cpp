#include "parser.hpp"

#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <sstream>

#include <xdgdirs.h>

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

bool Parser::operator ()(std::string line) // TODO: use Lua
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
            case Command::SETLOCAL: {
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
    std::string xdg_config_home = xdgConfigHome();
    if (xdg_config_home.empty()) {
        Editor().cli.error("XDG_CONFIG_HOME is set to invalid (empty) location");
        return;
    }

    std::string file{ xdg_config_home + "/sXe/init.lua" };

    if (!std::filesystem::exists(file)) {
        return;
    }

    luaL_dofile(L, file.c_str());
}


namespace sxe {

int set(lua_State* L)
{
    auto argc = lua_gettop(L);
    if (argc > 2) {
        Editor().cli.error("wrong number of arguments");
        return -1;
    }
    Editor().options.set(lua_tostring(L,1), lua_tostring(L,2));

    return 0;
}

}


Parser::Parser() : L(luaL_newstate())
{
    luaL_openlibs(L);
    lua_register(L, "set", sxe::set);

    std::transform(::commands.begin(), ::commands.end(),
                   std::inserter(commandsKeys, commandsKeys.begin()),
                   [](const auto& p) { return p.first; });
}

Parser::~Parser()
{
    lua_close(L);
}
