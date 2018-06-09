#include "archiver.h"
#include <time.h>

std::ifstream fin;
std::ofstream fout;

void print_code(bit_seq const &buffer) {
    uint32_t code_size = (buffer.size() + 7) / 8;

    for (size_t i = 0; i < code_size; i++) {
        fout << buffer.get_letter(i);
    }
}

void print_code_numbers(bit_seq const &buffer) {
    uint32_t code_size = (buffer.size() + 7) / 8 + 1;
    uint8_t last = static_cast<uint8_t>((buffer.size() % 8 == 0 ? 8 : (buffer.size() % 8)));
    bit_seq data;
    data.push_back(code_size);
    data.push_back(last);
    print_code(data);
}

void archiver::encode(std::string const &file_in, std::string const &file_out) {
    fin.open(file_in, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos_input = fin.tellg();

    if (pos_input == -1)
        throw std::runtime_error("File " + file_in + " doesn't exist");
    fin.seekg(0, std::ios::beg);

    try {
        fout.open(file_out, std::ios::binary);

        unsigned long long input_size = static_cast<unsigned long long int>(pos_input);
        Huffman_tree tree;

        while (input_size >= BUFFER_SIZE) {
            fin.read(buffer, BUFFER_SIZE);
            tree.add_letters(buffer, BUFFER_SIZE);
            input_size -= BUFFER_SIZE;
        }

        fin.read(buffer, static_cast<std::streamsize>(input_size));
        tree.add_letters(buffer, static_cast<size_t>(input_size));
        tree.build();

        bit_seq dictionary = tree.data_to_write();
        uint16_t dictionary_size = static_cast<uint16_t>(dictionary.size() / 8);

        bit_seq dict_size;
        dict_size.push_back(static_cast<uint8_t>(dictionary_size >> 8));
        dict_size.push_back(static_cast<uint8_t>(dictionary_size));

        print_code(dict_size);
        print_code(dictionary);

        fin.seekg(0, std::ios::beg);
        input_size = static_cast<unsigned long long int>(pos_input);
        bit_seq code;

        while (input_size >= BUFFER_SIZE) {
            fin.read(buffer, BUFFER_SIZE);
            code = tree.encode(buffer, BUFFER_SIZE);
            print_code_numbers(code);
            print_code(code);
            input_size -= BUFFER_SIZE;
        }

        fin.read(buffer, static_cast<std::streamsize>(input_size));
        code = tree.encode(buffer, static_cast<size_t>(input_size));
        print_code_numbers(code);
        print_code(code);

        fout.close();
        fin.close();
    } catch (std::runtime_error e) {
        fout.close();
        fin.close();
        throw;
    }
}

void archiver::decode(std::string const &file_in, std::string const &file_out) {
    long long time = clock();
    fin.open(file_in, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos_input = fin.tellg();

    if (pos_input == -1)
        throw std::runtime_error("File " + file_in + " doesn't exist");
    fin.seekg(0, std::ios::beg);

    try {
        fout.open(file_out, std::ios::binary);

        unsigned long long input_size = static_cast<unsigned long long int>(pos_input);
        if (input_size == 0) {
            fin.close();
            fout.close();
            return;
        }

        fin.read(buffer, 2);
        uint16_t dictionary_size = (static_cast<uint16_t>(buffer[0]) << 8) + (uint8_t) buffer[1];

        Huffman_tree tree;
        fin.read(buffer, dictionary_size);
        bit_seq dictionary;
        for (size_t i = 0; i < dictionary_size; i++) {
            dictionary.push_back((uint8_t) buffer[i]);
        }

        tree.build_from_data(dictionary);
        input_size -= 2 + dictionary_size;

        while (input_size > 0) {
            fin.read(buffer, 4);
            uint32_t buffer_size = ((((uint32_t) (uint8_t) buffer[0] << 8) +
                                     (uint32_t) (uint8_t) buffer[1] << 8) +
                                    (uint32_t) (uint8_t) buffer[2] << 8) +
                                   (uint32_t) (uint8_t) buffer[3];

            fin.read(buffer, buffer_size);
            bit_seq data;
            for (size_t i = 1; i < buffer_size; i++) {
                data.push_back((uint8_t) buffer[i]);
            }
            fout << tree.decode(data, (buffer_size - 2) * 8 + (uint8_t) buffer[0]);
            input_size -= buffer_size + 4;
        }

        fout.close();
        fin.close();
    } catch (std::runtime_error e) {
        fout.close();
        fin.close();
        throw;
    }
}


