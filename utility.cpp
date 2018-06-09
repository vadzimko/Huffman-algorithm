#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include "archiver/archiver.h"

std::string const input_rule = "Usage: encode/decode \"input\" \"output\"";

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << input_rule;
    } else {
        archiver arch;
        std::string command(argv[1], strlen(argv[1]));
        std::string file_in(argv[2], strlen(argv[2]));
        std::string file_out(argv[3], strlen(argv[3]));

        long long time = clock();
        if (command == "encode") {
            try {
                std::cout << "Encoding...\n" << std::flush;
                arch.encode(file_in, file_out);
                std::cout << "Encoded successfully: \'" << file_in << "\' to \'" << file_out << "\'" << std::endl;
            } catch (std::runtime_error e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } else if (command == "decode"){
            try {
                std::cout << "Decoding...\n" << std::flush;
                arch.decode(file_in, file_out);
                std::cout << "Decoded successfully: \'" << file_in << "\' to \'" << file_out << "\'" << std::endl;
            } catch (std::runtime_error e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Wrong command: \n" << argv[0] << "\"\n" << input_rule << std::endl;
            return 0;
        }
        std::cout << "Time: " << (double) (clock() - time) / 1000 << " sec" << std::endl;
    }
    return 0;
}