#pragma once

#define CTRL(x)  ((x) & 0x1f)

const char ESC = 27;

struct InitCurses {
    InitCurses();
    ~InitCurses();
};

struct EnableCursor {
    EnableCursor();
    ~EnableCursor();
};

char toPrintable(int);
