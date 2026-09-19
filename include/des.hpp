#pragma once

#include <array>
#include <string>
#include <vector>

namespace des {

class DES {
public:
    explicit DES(std::string key);

    std::string encrypt(const std::string& plaintextHex) const;
    std::string decrypt(const std::string& cipherTextHex) const;

private:
    std::vector<std::string> roundKeys_;

    template <std::size_t Size>
    std::string permute(const std::string& input, const std::array<int, Size>& positions) const;

    std::string leftShift(const std::string& value, int round) const;
    std::string xorBinary(const std::string& lhs, const std::string& rhs) const;
    std::string hexToBinary(const std::string& hexText) const;
    std::string binaryToHex(const std::string& binaryText) const;
    std::string fFunction(const std::string& right, const std::string& roundKey) const;
    std::string encryptOrDecrypt(const std::string& text, bool encrypt) const;
    std::string intToBinary4(int value) const;
    std::string makeRoundKey(const std::string& left, const std::string& right) const;
};

}  // namespace des
