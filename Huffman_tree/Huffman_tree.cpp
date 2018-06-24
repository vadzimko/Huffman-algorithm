#include <algorithm>
#include <vector>
#include <iostream>
#include <queue>
#include "Huffman_tree.h"

size_t const leave = 1000;

Huffman_tree::Huffman_tree() {
    std::fill(freq, freq + 256, 0);
}

void Huffman_tree::add_letters(char *data, size_t data_size) {
    for (size_t i = 0; i < data_size; i++) {
        freq[static_cast<uint8_t >(data[i])]++;
    }
}

void Huffman_tree::build() {
    std::vector<Node> array1;
    std::vector<Node> array2;
    for (size_t i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            Node node(leave, leave, freq[i], (uint8_t) i);
            array1.push_back(node);
        }
    }
    if (array1.size() == 0)
        return;

    std::sort(array1.begin(), array1.end(), Comparator);

    size_t tree_index = 1;

    size_t index1 = 0;
    size_t index2 = 0;
    for (size_t i = 0; i < array1.size() - 1; i++) {
        Node a;
        Node b;
        if (array1.size() == index1) {
            a = array2[index2++];
            b = array2[index2++];
        } else if (array2.size() == index2) {
            a = array1[index1++];
            b = array1[index1++];
        } else {
            if (array1[index1].weight < array2[index2].weight) {
                a = array1[index1++];
            } else {
                a = array2[index2++];
            }
            if (array1.size() == index1) {
                b = array2[index2++];
            } else if (array2.size() == index2) {
                b = array1[index1++];
            } else {
                if (array1[index1].weight < array2[index2].weight) {
                    b = array1[index1++];
                } else {
                    b = array2[index2++];
                }
            }
        }

        nodes[tree_index++] = a;
        nodes[tree_index++] = b;
        Node united(tree_index - 2, tree_index - 1, a.weight + b.weight, 0);
        array2.push_back(united);
    }

    nodes[0] = array1.size() == 1 ? array1.back() : array2.back();
    size = tree_index;
    set_codes();
}

void Huffman_tree::set_codes() {
    Node *root = get_root();
    if (root == nullptr)
        return;

    if (root->left == leave) {
        code[root->letter].push_back(false);
    } else {
        std::queue<std::pair<Node *, bit_seq>> q;
        q.push({root, bit_seq()});
        while (!q.empty()) {
            if (q.size() > 1000)
                throw std::runtime_error("File broken");
            Node *node = q.front().first;
            bit_seq node_code = q.front().second;
            q.pop();
            if (node->left != leave) {
                bit_seq left_code = node_code;
                bit_seq right_code = node_code;
                left_code.push_back(false);
                right_code.push_back(true);

                q.push({&nodes[node->left], left_code});
                q.push({&nodes[node->right], right_code});
            } else {
                code[node->letter] = node_code;
            }
        }
    }
}

void Huffman_tree::print() {
    if (get_root() == nullptr)
        return;

    printImpl(get_root());
}

void Huffman_tree::printImpl(Node *node) {
    if (node->left == leave) {
        std::cout << node->letter << ", count: " << node->weight << ", code: " << code[node->letter].get_bit_string()
                  << "\n";
    } else {
        printImpl(&nodes[node->left]);
        printImpl(&nodes[node->right]);
    }
}

bit_seq Huffman_tree::data_to_write() {
    if (get_root() == nullptr)
        return *(new bit_seq());
    bit_seq info;
    bit_seq leaves;
    dfs(info, leaves, get_root());

    while (info.get_last() % 8 != 0) {
        info.push_back(true);
    }
    uint16_t numbers = static_cast<uint16_t>(info.size() / 8);

    bit_seq result;
    result.push_back(static_cast<uint8_t >(numbers >> 8));
    result.push_back(static_cast<uint8_t >(numbers));
    result.push_back(info);
    result.push_back(leaves);
    return result;
}

void Huffman_tree::dfs(bit_seq &info, bit_seq &leaves, Node *node) {
    info.push_back(false);
    if (node->left == leave) {
        leaves.push_back((uint8_t) node->letter);
    } else {
        dfs(info, leaves, &nodes[node->left]);
        dfs(info, leaves, &nodes[node->right]);
    }
    info.push_back(true);
}

void Huffman_tree::build_from_data(bit_seq const &data) {
    if (data.size() == 0) {
        return;
    }

    size_t number = static_cast<uint8_t >(data.get_letter(1));
    number += ((uint32_t) data.get_letter(0) << 8);
    size_t info_index = 17;
    size_t words_index = 2 + number;

    size_t node_index = 0;
    buildImpl(data, info_index, words_index, node_index);
    size = node_index + 1;
    set_codes();
}

void Huffman_tree::buildImpl(bit_seq const &data, size_t &info_index, size_t &words_index, size_t &nodes_index) {
    if (data[info_index++]) {
        nodes[nodes_index].letter = static_cast<uint8_t>(data.get_letter(words_index++));
        nodes[nodes_index].left = nodes[nodes_index].right = leave;
    } else {
        size_t curr_index = nodes_index;
        ++nodes_index;
        nodes[curr_index].left = nodes_index;
        buildImpl(data, info_index, words_index, nodes_index);
        if (!data[info_index++]) {
            ++nodes_index;
            nodes[curr_index].right = nodes_index;
            buildImpl(data, info_index, words_index, nodes_index);
        }
        ++info_index;
    }
}

bit_seq Huffman_tree::encode(char const *data, size_t size) {
    bit_seq result;
    for (size_t i = 0; i < size; i++)
        result.push_back(code[(int) static_cast<uint8_t>(data[i])]);
    return result;
};

std::string Huffman_tree::decode(bit_seq const &data, size_t size) {
    Node *root = get_root();
    if (root == nullptr) {
        return "";
    }
    std::string result;

    Node *node = root;
    for (size_t i = 0; i < size; i++) {
        if (node->left != leave) {
            if (!data[i])
                node = &nodes[node->left];
            else node = &nodes[node->right];
        }
        if (node->left == leave) {
            result.push_back(node->letter);
            node = root;
        }
    }
    return result;
}