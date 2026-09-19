#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "des.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <encrypt|decrypt> <hex-data> <key>\n";
        return 1;
    }

    const std::string mode = argv[1];
    const std::string data = argv[2];
    const std::string key = argv[3];

    try {
        des::DES cipher(key);

        if (mode == "encrypt") {
            std::cout << cipher.encrypt(data) << '\n';
            return 0;
        }

        if (mode == "decrypt") {
            std::cout << cipher.decrypt(data) << '\n';
            return 0;
        }

        std::cerr << "Mode must be either 'encrypt' or 'decrypt'.\n";
        return 1;
    } catch (const std::exception& exception) {
        std::cerr << "DES error: " << exception.what() << '\n';
        return 1;
    }
}
