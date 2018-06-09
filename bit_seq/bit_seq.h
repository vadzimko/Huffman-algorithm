#ifndef H_BIT_SEQ_H
#define H_BIT_SEQ_H

#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>

struct bit_seq {
    bit_seq() : data(1, 0), last(0) {}
    bit_seq(bit_seq const& other);
    ~bit_seq() = default;

    size_t get_data_size() const { return data.size();}
    size_t get_last() const { return last;}
    size_t size() const { return (data.size() - 1) * 32 + last;}

    void push_back(bool x);
    void push_back(uint8_t x);
    void push_back(uint32_t x);
    void push_back(bit_seq const &other);

    bool operator[](size_t index) const { return (data[index / 32] & (1 << (31 - index % 32))) != 0;}
    char get_letter(size_t index) const { return static_cast<char>((data[index / 4] >> ((3 - index % 4) * 8)) & ((1 << 8) - 1));}
    uint32_t get_number(size_t index) const { return data[index];}

    void check_last();
    std::string get_bit_string();
private:
    std::vector<uint32_t> data;
    size_t last;
};
#endif //H_BIT_SEQ_H
