#include "des.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

#include "des_constants.hpp"

namespace des {

namespace {

int bitsToValue(const std::string& bits) {
    int value = 0;
    for (char bit : bits) {
        value = (value << 1) | (bit == '1' ? 1 : 0);
    }
    return value;
}

std::string binaryToHexNibble(const std::string& nibble) {
    static const std::array<char, 16> values = {'0', '1', '2', '3', '4', '5', '6', '7',
                                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    const auto index = bitsToValue(nibble);
    return std::string(1, values[static_cast<std::size_t>(index)]);
}

}  // namespace

DES::DES(std::string key) {
    std::transform(key.begin(), key.end(), key.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });

    if (key.empty()) {
        throw std::invalid_argument("DES key must not be empty.");
    }

    const std::string binaryKey = hexToBinary(key);
    const std::string permutedKey = permute(binaryKey, constants::PARITY_DROP);
    std::string left = permutedKey.substr(0, 28);
    std::string right = permutedKey.substr(28, 28);

    for (int round = 1; round <= 16; ++round) {
        left = leftShift(left, round);
        right = leftShift(right, round);
        roundKeys_.push_back(makeRoundKey(left, right));
    }
}

std::string DES::encrypt(const std::string& plaintextHex) const {
    return encryptOrDecrypt(plaintextHex, true);
}

std::string DES::decrypt(const std::string& cipherTextHex) const {
    return encryptOrDecrypt(cipherTextHex, false);
}

template <std::size_t Size>
std::string DES::permute(const std::string& input, const std::array<int, Size>& positions) const {
    std::string result;
    result.reserve(positions.size());

    for (const int index : positions) {
        result.push_back(input[static_cast<std::size_t>(index) - 1]);
    }

    return result;
}

std::string DES::leftShift(const std::string& value, int round) const {
    if (round == 1 || round == 2 || round == 9 || round == 16) {
        return value.substr(1) + value[0];
    }
    return value.substr(2) + value.substr(0, 2);
}

std::string DES::xorBinary(const std::string& lhs, const std::string& rhs) const {
    if (lhs.size() != rhs.size()) {
        throw std::invalid_argument("XOR requires operands of the same size.");
    }

    std::string result;
    result.reserve(lhs.size());
    for (std::size_t index = 0; index < lhs.size(); ++index) {
        result.push_back(lhs[index] == rhs[index] ? '0' : '1');
    }
    return result;
}

std::string DES::hexToBinary(const std::string& hexText) const {
    std::string binary;
    binary.reserve(hexText.size() * 4);

    for (const char value : hexText) {
        const char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
        switch (upper) {
            case '0': binary += "0000"; break;
            case '1': binary += "0001"; break;
            case '2': binary += "0010"; break;
            case '3': binary += "0011"; break;
            case '4': binary += "0100"; break;
            case '5': binary += "0101"; break;
            case '6': binary += "0110"; break;
            case '7': binary += "0111"; break;
            case '8': binary += "1000"; break;
            case '9': binary += "1001"; break;
            case 'A': binary += "1010"; break;
            case 'B': binary += "1011"; break;
            case 'C': binary += "1100"; break;
            case 'D': binary += "1101"; break;
            case 'E': binary += "1110"; break;
            case 'F': binary += "1111"; break;
            default:
                throw std::invalid_argument("Invalid hexadecimal character in DES input.");
        }
    }

    return binary;
}

std::string DES::binaryToHex(const std::string& binaryText) const {
    if (binaryText.size() % 4 != 0) {
        throw std::invalid_argument("Binary text length must be divisible by 4 for hexadecimal conversion.");
    }

    std::string hex;
    hex.reserve(binaryText.size() / 4);

    for (std::size_t index = 0; index < binaryText.size(); index += 4) {
        hex += binaryToHexNibble(binaryText.substr(index, 4));
    }

    return hex;
}

std::string DES::fFunction(const std::string& right, const std::string& roundKey) const {
    const std::string expanded = permute(right, constants::EXPANSION);
    const std::string xorValue = xorBinary(expanded, roundKey);

    std::string sBoxOutput;
    sBoxOutput.reserve(32);

    for (std::size_t index = 0; index < xorValue.size(); index += 6) {
        const std::string block = xorValue.substr(index, 6);
        const std::string rowBits = block.substr(0, 1) + block.substr(5, 1);
        const std::string columnBits = block.substr(1, 4);
        const int row = bitsToValue(rowBits);
        const int column = bitsToValue(columnBits);
        const int value = constants::S_BOX[index / 6][static_cast<std::size_t>(row)][static_cast<std::size_t>(column)];
        sBoxOutput += intToBinary4(value);
    }

    return permute(sBoxOutput, constants::STRAIGHT_PERMUTATION);
}

std::string DES::encryptOrDecrypt(const std::string& text, bool encrypt) const {
    const std::string binaryData = hexToBinary(text);
    const std::string permuted = permute(binaryData, constants::INITIAL_PERMUTATION);

    std::string left = permuted.substr(0, 32);
    std::string right = permuted.substr(32, 32);

    for (std::size_t round = 0; round < 16; ++round) {
        const std::string roundKey = encrypt ? roundKeys_[round] : roundKeys_[15 - round];
        const std::string fValue = fFunction(right, roundKey);
        const std::string nextLeft = xorBinary(fValue, left);

        if (round != 15) {
            std::swap(left, right);
            right = nextLeft;
        } else {
            left = nextLeft;
        }
    }

    const std::string combined = left + right;
    const std::string finalBlock = permute(combined, constants::FINAL_PERMUTATION);
    return binaryToHex(finalBlock);
}

std::string DES::intToBinary4(int value) const {
    std::string result;
    for (int bit = 3; bit >= 0; --bit) {
        result.push_back(((value >> bit) & 1) ? '1' : '0');
    }
    return result;
}

std::string DES::makeRoundKey(const std::string& left, const std::string& right) const {
    const std::string combined = left + right;
    return permute(combined, constants::KEY_COMPRESSION);
}

template std::string DES::permute<64>(const std::string& input, const std::array<int, 64>& positions) const;
template std::string DES::permute<56>(const std::string& input, const std::array<int, 56>& positions) const;
template std::string DES::permute<48>(const std::string& input, const std::array<int, 48>& positions) const;
template std::string DES::permute<32>(const std::string& input, const std::array<int, 32>& positions) const;

}  // namespace des
