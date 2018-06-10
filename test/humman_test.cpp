#include <cstring>
#include <fstream>

#include "../Huffman_tree/Huffman_tree.h"
#include "gtest.h"
#include "../archiver/archiver.h"

archiver arch;

char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
std::string const temp_file = "temp.huf";
std::string const decoded = "result.huf_decoded";

bool check_equal(std::string const &file, std::string const &encoded_decoded) {
    std::ifstream first(file, std::ios::binary | std::ios::ate);
    std::ifstream second(encoded_decoded, std::ios::binary | std::ios::ate);

    std::ifstream::pos_type first_input = first.tellg();
    std::ifstream::pos_type second_input = second.tellg();
    if (first_input == -1 || second_input == -1 || first_input != second_input) {
        return false;
    }
    first.seekg(0, std::ios::beg);
    second.seekg(0, std::ios::beg);

    unsigned long long size = static_cast<unsigned long long int>(first_input);
    while (BUFFER_SIZE <= size) {
        size -= BUFFER_SIZE;
        first.read(buffer1, BUFFER_SIZE);
        second.read(buffer2, BUFFER_SIZE);
        if (!std::equal(buffer1, buffer1 + BUFFER_SIZE, buffer2)) {
            return false;
        }
    }

    first.read(buffer1, static_cast<std::streamsize>(size));
    second.read(buffer2, static_cast<std::streamsize>(size));
    return std::equal(buffer1, buffer1 + size, buffer2);
}


bool encode_decode_file(std::string file_in) {
    arch.encode(file_in, temp_file);
    arch.decode(temp_file, decoded);

    bool equals = check_equal(file_in, decoded);

    remove(temp_file.c_str());
    remove(decoded.c_str());
    return equals;
}

TEST(correctness, checking_empty) {

    EXPECT_TRUE(encode_decode_file("../test_files/empty.txt"));
}

TEST(correctness, checking_small) {

    EXPECT_TRUE(encode_decode_file("../test_files/simple.txt"));
}

TEST(correctness, checking_medium) {

    EXPECT_TRUE(encode_decode_file("../test_files/bible.txt"));
}

//TEST(correctness, checking_big) {
//
//    EXPECT_TRUE(encode_decode_file("../test_files/test.txt"));
//}

TEST(correctness, checking_white_square_image) {

    EXPECT_TRUE(encode_decode_file("../test_files/white.png"));
}

TEST(correctness, checking_cool_photo) {

    EXPECT_TRUE(encode_decode_file("../test_files/tanos.jpg"));
}
//
//TEST(correctness, checking_big_pdf) {
//
//    EXPECT_TRUE(encode_decode_file("../test_files/book.pdf"));
//}
//
//TEST(correctness, checking_very_big_pdf) {
//
//    EXPECT_TRUE(encode_decode_file("../test_files/english.pdf"));
//}
//
//TEST(correctness, checking_video) {
//
//    EXPECT_TRUE(encode_decode_file("../test_files/video.avi"));
//}
