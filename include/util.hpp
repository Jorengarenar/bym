#pragma once

struct InitCurses {
    InitCurses();
    ~InitCurses();
};

struct EnableCursor {
    EnableCursor();
    ~EnableCursor();
};

char toPrintable(int);
