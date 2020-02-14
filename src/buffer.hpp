#pragma once

#include <vector>

class Buffer {
  public:
    std::vector<unsigned char> bytes;
    Buffer();
    Buffer(char*);
    void print();
};
