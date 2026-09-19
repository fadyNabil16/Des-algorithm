#include <cassert>
#include <iostream>
#include <string>

#include "des.hpp"

int main() {
    const std::string key = "133457799BBCDFF1";
    const std::string plaintext = "0123456789ABCDEF";
    const std::string ciphertext = "85E813540F0AB405";

    des::DES cipher(key);

    const std::string encrypted = cipher.encrypt(plaintext);
    assert(encrypted == ciphertext);

    const std::string decrypted = cipher.decrypt(ciphertext);
    assert(decrypted == plaintext);

    const std::string anotherPlaintext = "0000000000000000";
    const std::string roundTrip = cipher.decrypt(cipher.encrypt(anotherPlaintext));
    assert(roundTrip == anotherPlaintext);

    bool invalidKeyCaught = false;
    try {
        des::DES invalid("");
        (void)invalid;
    } catch (const std::exception&) {
        invalidKeyCaught = true;
    }
    assert(invalidKeyCaught);

    std::cout << "DES tests passed." << std::endl;
    return 0;
}
