#include <iostream>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

class DES
{
private:
    int init_per[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                         60, 52, 44, 36, 28, 20, 12, 4,
                         62, 54, 46, 38, 30, 22, 14, 6,
                         64, 56, 48, 40, 32, 24, 16, 8,
                         57, 49, 41, 33, 25, 17, 9, 1,
                         59, 51, 43, 35, 27, 19, 11, 3,
                         61, 53, 45, 37, 29, 21, 13, 5,
                         63, 55, 47, 39, 31, 23, 15, 7 };

    int final_per[64] = {
                            40, 8, 48, 16, 56, 24, 64, 32,
                            39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30,
                            37, 5, 45, 13, 53, 21, 61, 29,
                            36, 4, 44, 12, 52, 20, 60, 28,
                            35, 3, 43, 11, 51, 19, 59, 27,
                            34, 2, 42, 10, 50, 18, 58, 26,
                            33, 1, 41, 9, 49, 17, 57, 25
    };

    map<char, string> alloc_hex = {
                                    {'0' , "0000"},
                                    {'1' , "0001"},
                                    {'2' , "0010"},
                                    {'3' , "0011"},
                                    {'4' , "0100"},
                                    {'5' , "0101"},
                                    {'6' , "0110"},
                                    {'7' , "0111"},
                                    {'8' , "1000"},
                                    {'9' , "1001"},
                                    {'A' , "1010"},
                                    {'B' , "1011"},
                                    {'C' , "1100"},
                                    {'D' , "1101"},
                                    {'E' , "1110"},
                                    {'F' , "1111"},
                {'a' , "1010"},
                                    {'b' , "1011"},
                                    {'c' , "1100"},
                                    {'d' , "1101"},
                                    {'e' , "1110"},
                                    {'f' , "1111"}
    };
    map<string, char> alloc_bin = {
                                     {"0000", '0'},
                                     {"0001", '1'},
                                     {"0010",'2'},
                                     {"0011",'3' },
                                     {"0100", '4'},
                                     {"0101", '5'},
                                     {"0110", '6'},
                                     {"0111", '7'},
                                     {"1000", '8'},
                                     {"1001", '9'},
                                     {"1010", 'A'},
                                     {"1011", 'B'},
                                     {"1100", 'C'},
                                     {"1101", 'D'},
                                     {"1110", 'E'},
                                     {"1111", 'F'},
    };
    int s_box[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };
    int parity_drop[56] = { 57, 49, 41, 33, 25, 17, 9,
                 1, 58, 50, 42, 34, 26, 18,
                 10, 2, 59, 51, 43, 35, 27,
                 19, 11, 3, 60, 52, 44, 36,
                 63, 55, 47, 39, 31, 23, 15,
                 7, 62, 54, 46, 38, 30, 22,
                 14, 6, 61, 53, 45, 37, 29,
                 21, 13, 5, 28, 20, 12, 4 };
    int comparison_d[48] = { 14, 17, 11, 24, 1, 5,
                             3, 28, 15, 6, 21, 10,
                             23, 19, 12, 4, 26, 8,
                             16, 7, 27, 20, 13, 2,
                             41, 52, 31, 37, 47, 55,
                             30, 40, 51, 45, 33, 48,
                             44, 49, 39, 56, 34, 53,
                             46, 42, 50, 36, 29, 32 };
    int expan_d[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                  6, 7, 8, 9, 8, 9, 10, 11,
                  12, 13, 12, 13, 14, 15, 16, 17,
                  16, 17, 18, 19, 20, 21, 20, 21,
                  22, 23, 24, 25, 24, 25, 26, 27,
                  28, 29, 28, 29, 30, 31, 32, 1 };
    int straight_d[32] = { 16, 7, 20, 21,
                29, 12, 28, 17,
                1, 15, 23, 26,
                5, 18, 31, 10,
                2, 8, 24, 14,
                32, 27, 3, 9,
                19, 13, 30, 6,
                22, 11, 4, 25 };
    string* arr = new string[16];
public:
    DES(string key)
    {
        this->key_gen(this->hex_to_bin(key));
    }
    string shifting(string str, int rond) {
        if (rond == 1 || rond == 2 || rond == 9 || rond == 16) {
            string pass = str.substr(1, 27) + str[0];
            return pass;
        }
        else {
            string pass = str.substr(2, 27) + str[0] + str[1];
            return pass;
        }
    }
    void key_gen(string str) {
        string before = "";
        int j = 0;
        for (int i = 0; i < 56; i++) {
            //if((i+1)%8 == 0) continue;
            before += str[parity_drop[i] - 1];
        }
        string left = before.substr(0, 28);
        string right = before.substr(28, 28);
        string key_g = "", total = "";
        for (int i = 1; i <= 16; i++) {
            left = shifting(left, i);
            right = shifting(right, i);
            key_g = "", total = "";
            key_g += (left);
            key_g += (right);
            for (int k = 0; k < 48; k++) {
                total += key_g[comparison_d[k] - 1];
            }
            arr[i - 1] = total;
        }
    }
    string int_to_bin(int num) {
        char r;
        if (num >= 10) {
            r = (num % 10) + 65;
        }
        else {
            r = num + 48;
        }
        return alloc_hex.find(r)->second;
    }
    string hex_to_bin(string str)
    {
        string after_per = "";
        for (char& c : str) {
            after_per += alloc_hex.find(c)->second;
        }
        return after_per;
    }
    string expansion_d(string str)
    {
        string expanded = "";
        expanded += str[31];
        for (int i = 0; i < str.length(); i++) {
            if ((i + 1) % 4 == 0 && i != 31) {
                expanded += str[i];
                expanded += str[i + 1];
                expanded += str[i];
            }
            else {
                expanded += str[i];
            }
        }
        expanded += str[0];
        string half = "";
        for (int j = 0; j < 48; j++) {
            half += expanded[expan_d[j] - 1];
        }
        return half;
    }
    string do_s_box(string str)
    {
        string _str = "";
        int loop = 0, pos;
        for (int i = 0; i < str.length(); i += 6)
        {
            string inner = str.substr(i, 6);
            char c1 = inner[0], c2 = inner[5];
            char c3 = alloc_bin.find(inner.substr(1, 4))->second;
            if (int(c3) < 65) {
                pos = int(c3) % 16;
            }
            else {
                pos = (int(c3) % 16) + 9;
            }
            string ss = "";
            ss += c1;
            ss += c2;
            int _pos = (ss == "00" ? 0 : ss == "01" ? 1 : ss == "10" ? 2 : 3);

            int _num = s_box[loop][_pos][pos];
            _str += int_to_bin(_num);
            loop++;
        }
        return _str;
    }
    string bin_to_hex(string str) {
        string hex = "";
        for (int i = 0; i < str.length(); i += 4)
        {
            hex += alloc_bin.find(str.substr(i, 4))->second;
        }
        return hex;
    }
    string XOR(string str1, string str2)
    {
        string xoring = "";
        for (int i = 0; i < str2.length(); i++) {
            if (str1[i] == str2[i]) {
                xoring += "0";
            }
            else {
                xoring += "1";
            }
        }
        return xoring;
    }
    string f_box(string right, int roun, string type) {
        string after = "";
        string finish_f = "";
        for (int i = 0; i < 48; i++) {
            after += right[expan_d[i] - 1];
        }
        string aftre_xor = "";
        if (type == "enc")aftre_xor = XOR(after, arr[roun - 1]);
        else aftre_xor = XOR(after, arr[16 - roun]);
        string after_s = do_s_box(aftre_xor);
        for (int i = 0; i < 32; i++) {
            finish_f += after_s[straight_d[i] - 1];
        }
        return finish_f;
    }
    void des(string* data/*, string key, string method*/)
    {
        string binary_data = hex_to_bin(*data);
        //string *sds = new string(binary_data);
        string str = "", hex = "", swap = "";
        int i = 0;
        for (char& c : binary_data) {
            str += binary_data[init_per[i] - 1];
            i++;
        }
        string left_part = str.substr(0, 32);
        string right_part = str.substr(32, 32);
        for (int i = 1; i <= 16; i++) {
            string f__box = f_box(right_part, i, "enc");
            left_part = XOR(f__box, left_part);
            //cout<<bin_to_hex(f__box)<<endl;
            if (i != 16)left_part.swap(right_part);
        }
        string done = left_part + right_part;
        string done2 = "";
        for (int i = 0; i < 64; i++)
        {
            done2 += done[final_per[i] - 1];
        }
        cout << "cipher: " << bin_to_hex(done2);

    }
    void invers_des(string* data) {
        string binary_data = hex_to_bin(*data);
        string str = "", hex = "", swap = "";
        int i = 0;
        for (char& c : binary_data) {
            str += binary_data[init_per[i] - 1];
            i++;
        }
        string left_part = str.substr(0, 32);
        string right_part = str.substr(32, 32);
        for (int i = 1; i <= 16; i++) {
            string f__box = f_box(right_part, i, "dec");
            left_part = XOR(f__box, left_part);
            //cout<<bin_to_hex(f__box)<<endl;
            if (i != 16)left_part.swap(right_part);
        }
        string done = left_part + right_part;
        string done2 = "";
        for (int i = 0; i < 64; i++)
        {
            done2 += done[final_per[i] - 1];
        }
        cout << "plain: " << bin_to_hex(done2);
    }
};

int main(int argc, char* argv[]) {
    string sty = argv[1];
    string data = argv[2];
    string key = argv[3];
    //C0B7A8D05F3A829C    123456ABCD132536
    DES* myobj = new DES(key);
    if (sty == "encrypt") myobj->des(&data);
    else if (sty == "decrypt") myobj->invers_des(&data);
}