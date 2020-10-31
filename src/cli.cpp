#include "cli.hpp"

#include <algorithm>

#include <ncurses.h>

#include "parser.hpp"
#include "util.hpp"
#include "editor.hpp"

Cli::Cli()
{
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

Cli::~Cli()
{
    delwin(line);
}

void Cli::redraw()
{
    delwin(line);
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

bool Cli::complete(std::string& buf, int& b, int& i)
{
    const int y = getcury(line);

    const std::string& prefix = getLastWord(buf);

    auto& keys = Editor().parser.commandsKeys; /* TODO: change it, so it will
                                                complete also command arguments */

    auto it = keys.lower_bound(prefix);

    if (*it == prefix) {
        ++it;
    }

    std::string completed{ prefix };

    // Erase previous completion
    auto erase = [&]() {
        int n = i - completed.length();

        for (; i > n; --i) {
            mvwdelch(line, y, i);
        }

        buf.erase(i, i + completed.length());
    };

    do {
        erase();

        if (it != keys.end() && isPrefix(prefix, *it)) {
                completed = *it;
        }
        else {
            completed = prefix;
        }

        // Set new completion
        i += completed.length();
        buf += completed;
        wprintw(line, completed.c_str());

        if (completed == prefix) {
            return false;
        }

        ++it;

        b = wgetch(line);
    } while (b == '\t');

    return true;
}

std::string Cli::input()
{
    std::string buf{};
    int b = 0;
    int y = getcury(line);
    bool skipB = false;

    EnableCursor c;
    for (int i = 0; b != '\n'; ) {
        if (!skipB) {
            b = wgetch(line);
        }
        else {
            skipB = false;
        }

        // TODO: line wrap

        switch (b) {
            case '\n':
                break;

            case ::ESC:
            case CTRL('c'):
                return "";

            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (i) {
                    mvwdelch(line, y, i);
                    buf.pop_back();
                    --i;
                }
                break;

            case '\t':
                skipB = complete(buf, b, i);
                break;

            default:
                wprintw(line, "%c", b);
                buf += b;
                ++i;

        }

    }

    return buf;
}

bool Cli::operator ()()
{
    wclear(line);
    mvwprintw(line, 0, 0, ":");
    wrefresh(line);

    std::string buf = input();

    if (buf.empty()) {
        wclear(line);
        wrefresh(line);
        return true;
    }

    return Editor().parser(buf);
}

void Cli::error(std::string msg)
{
    wclear(line);
    mvwprintw(line, 0, 0, msg.c_str());
    wrefresh(line);
}
