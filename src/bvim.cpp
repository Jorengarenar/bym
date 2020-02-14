#include <ncurses.h>
#include "buffer.hpp"

int main(int argc, char* argv[])
{
    if (argc == 1) {
        puts("Usage: bvim <file>");
    }
    else {
        initscr();
        scrollok(stdscr, TRUE);
        Buffer buf(argv[1]);
        buf.print();
        refresh();
        getch();
        endwin();
    }
}
