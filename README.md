# DES Algorithm Refactor

This project contains a cleaned-up C++ implementation of the Data Encryption Standard (DES).

## Project structure

- `src/main.cpp` — command-line entry point
- `src/des.cpp` — DES logic
- `include/des.hpp` — public class interface
- `include/des_constants.hpp` — DES permutation tables, key schedule values, and S-box data
- `tests/test_des.cpp` — automated validation tests

## Build

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/des.cpp src/main.cpp -o des_app
```

## Run

Encrypt:

```bash
./des_app encrypt 0123456789ABCDEF 133457799BBCDFF1
```

Decrypt:

```bash
./des_app decrypt 85E813540F0AB405 133457799BBCDFF1
```

## Test

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_des.cpp src/des.cpp -o test_des && ./test_des
```

## Example output

```text
85E813540F0AB405
0123456789ABCDEF
```

This project keeps the original DES functionality while improving code organization, constant separation, and maintainability.
