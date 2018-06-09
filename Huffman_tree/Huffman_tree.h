#ifndef H_HUFFMAN_TREE_H
#define H_HUFFMAN_TREE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include "../bit_seq/bit_seq.h"

struct Huffman_tree {
private:
    bit_seq code[512];
    size_t freq[256];

    struct Node {
        size_t left;
        size_t right;
        int weight;
        uint8_t letter;

        Node() : left(0), right(0), weight(0), letter(static_cast<uint8_t>(0)) {}

        Node(size_t left, size_t right, int weight, char letter)
                : left(left), right(right), weight(weight), letter(static_cast<uint8_t>(letter)) {}

    };
    static bool Comparator(Node &a, Node &b) {
        return a.weight < b.weight;
    }

    Node nodes[512];
    size_t size = 0;

public:
    Huffman_tree();
    ~Huffman_tree() = default;

    Node *get_root() {
        if (size == 0)
            return nullptr;
        return &nodes[0];
    }
    size_t get_size() { return size; }

    void add_letters(char *data, size_t data_size);
    void build();
    void set_codes();

    void print();

    bit_seq data_to_write();
    void dfs(bit_seq &info, bit_seq &leaves, Node *node);

    void build_from_data(bit_seq const &data);

    bit_seq encode(char const *data, size_t size);
    std::string decode(bit_seq const &data, size_t size);

private:
    void printImpl(Node *node);
    void buildImpl(bit_seq const &data, size_t &info_index, size_t &words_index, size_t &nodes_index);
};


#endif //H_HUFFMAN_TREE_H
