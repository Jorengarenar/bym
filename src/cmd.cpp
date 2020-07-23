#include "cmd.hpp"

#include <algorithm>
#include <ranges>

#include <ncurses.h>

#include "parser.hpp"
#include "util.hpp"
#include "editor.hpp"

Cmd::Cmd(Editor& e) : editor(e)
{
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

Cmd::~Cmd()
{
    delwin(line);
}

void Cmd::redraw()
{
    delwin(line);
    line = newwin(1, COLS, LINES-1, 0);
    wrefresh(line);
}

bool Cmd::complete(std::string& buf, const std::string& last, int& b, int& i)
{
    int y = getcury(line);

    auto& keys = editor.parser.commandsKeys;

    auto first = keys.lower_bound(last);

    if (first == keys.end()) {
        return false;
    }

    auto it = first;

    auto findCompletion = [&](std::string& b) {
        if (it != keys.end()) {
            i += it->length();
            b = *it;
        }
        return b;
    };

    std::string completed{ last };

    auto erase = [&]() {
        int n = i - completed.length();

        for (; i > n; --i) {
            mvwdelch(line, y, i);
        }

        buf.erase(i, i + completed.length());
    };

    erase();
    buf += findCompletion(completed);
    wprintw(line, completed.c_str());

    b = wgetch(line);

    while (b == '\t') {
        erase();

        ++it;

        if (it == keys.end()) {
            it = first;
        }
        else if (!isPrefix(last, *it)) {
            it = first;
        }

        completed = *it;
        i += completed.length();
        buf += completed;
        wprintw(line, completed.c_str());

        b = wgetch(line);
    }

    return true;
}

std::string Cmd::input()
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

        skipB = false;

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
                skipB = complete(buf, getLastWord(buf), b, i);
                break;

            default:
                wprintw(line, "%c", b);
                buf += b;
                ++i;

        }

    }

    return buf;
}

bool Cmd::operator ()()
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

    return editor.parser(buf);
}

void Cmd::error(std::string msg)
{
    wclear(line);
    mvwprintw(line, 0, 0, msg.c_str());
    wrefresh(line);
}
