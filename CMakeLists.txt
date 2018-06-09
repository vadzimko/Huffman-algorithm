cmake_minimum_required(VERSION 3.9)
project(h)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/huffman_lib)

add_library(huffman_library STATIC
        Huffman_tree/Huffman_tree.cpp
        Huffman_tree/Huffman_tree.h
        bit_seq/bit_seq.cpp
        bit_seq/bit_seq.h
)

add_executable(huffman
        utility.cpp
        archiver/archiver.cpp
        archiver/archiver.h)

add_executable(huffman_test
        test/humman_test.cpp
        test/gtest.h
        test/gtest-all.cc
        test/gtest_main.cc
        archiver/archiver.cpp
        archiver/archiver.h)

target_link_libraries(huffman huffman_library)
target_link_libraries(huffman_test huffman_library)