#include <iostream>

#include <unistd.h>

#include "editor.hpp"

void usage()
{
}

int main(int argc, char* argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                std::cout << "0.0.1" << std::endl;
                return 0;
            case 'h':
                usage();
                return 0;
            case '?':
                std::cout << "Unknown option: " << optopt << std::endl;
                return 1;
            default:
                abort();
        }
    }

    Editor editor{ argc, argv, optind };
    while (editor()) {}

    return 0;
}

// vim: fen
