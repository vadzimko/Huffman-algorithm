#include <iostream>
#include "bit_seq.h"

bit_seq::bit_seq(bit_seq const &other)
        : data(other.data), last(other.last) {}

void bit_seq::push_back(bool x) {
    data[data.size() - 1] |= (x << (31 - last));
    last++;
    check_last();
}

void bit_seq::push_back(uint8_t x) {
    if (last + 8 > 32) {
        data[data.size() - 1] |= (x >> (last - 24));
        data.push_back(0);
        last %= 8;

        data[data.size() - 1] |= (x << (32 - last));
    } else {
        data[data.size() - 1] |= (x << (24 - last));
        last += 8;
    }
    check_last();
}

void bit_seq::push_back(uint32_t x) {
    if (last == 0) {
        data[data.size() - 1] = x;
        last += 32;
    } else {
        data[data.size() - 1] |= (x >> last);
        data.push_back(0);

        data[data.size() - 1] |= (x << (32 - last));
    }
    check_last();
}

void bit_seq::push_back(bit_seq const &other) {
    size_t cur_index = 0;

    while (cur_index < other.get_data_size() - 1) {
        push_back(other.data[cur_index]);
        cur_index++;
    }

    if (last + other.get_last() <= 32) {
        data[data.size() - 1] |= other.data[cur_index] >> last;
        last = last + other.get_last();
        check_last();
    } else {
        push_back(other.data[cur_index]);
        last -= 32 - other.get_last();
    }
}

void bit_seq::check_last() {
    if (last >= 32) {
        data.push_back(0);
        last -= 32;
    }
}

std::string bit_seq::get_bit_string() {
    std::string res;
    std::string str;
    uint32_t temp;
    for (size_t i = 0; i < data.size() - 1; i++) {
        temp = data[i];
        for (size_t j = 0; j < 32; j++) {
            str += '0' + temp % 2;
            temp >>= 1;
        }
        std::reverse(str.begin(), str.end());
        res += str + " ";
        str.clear();
    }
    temp = data[data.size() - 1] >> (32 - last);
    for (size_t j = 0; j < last; j++) {
        str += '0' + temp % 2;
        temp >>= 1;
    }
    std::reverse(str.begin(), str.end());
    res += str;
    return res;
}