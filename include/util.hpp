#pragma once

const char ESC = 27;

struct InitCurses {
    InitCurses();
    ~InitCurses();
};

struct EnableCursor {
    EnableCursor();
    EnableCursor(int);
    ~EnableCursor();
};

char toPrintable(int);
