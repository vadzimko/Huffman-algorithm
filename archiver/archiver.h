#ifndef H_ARCHIVER_H
#define H_ARCHIVER_H

#include <bits/ios_base.h>
#include <iostream>
#include "../Huffman_tree/Huffman_tree.h"
#include <fstream>

static const size_t BUFFER_SIZE = 1 << 20;
static char buffer[BUFFER_SIZE << 1];

class archiver {
public:
    void encode(std::string const &file_in, std::string const &file_out);
    void decode(std::string const &file_in, std::string const &file_out);
    ~archiver() = default;
};


#endif //H_ARCHIVER_H
