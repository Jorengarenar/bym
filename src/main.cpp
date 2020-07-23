#include <iostream>

#include <unistd.h>

#include "editor.hpp"

void usage()
{
    std::cout << PROJECT_NAME << " (" << VERSION << ")" << std::endl;
}

int main(int argc, char* argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "vh")) != -1) {
        switch (opt) {
            case 'v':
                std::cout << VERSION << std::endl;
                return 0;
            case 'h':
                usage();
                return 0;
            case '?':
                return 1;
            default:
                abort();
        }
    }

    Editor editor{ argc, argv, optind };
    while (editor()) {}

    return 0;
}
