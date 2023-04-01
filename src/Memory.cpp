//
// Created by kime on 01.04.2023.
//

#include "Memory.h"
#include <cstring>
#include <fstream>

uint64_t Memory::read(uint16_t address)
{
    return uint64_t(memory[address]) << 48 | uint64_t(memory[address + 16]) << 32 | uint64_t(memory[address + 32]) << 16 | uint64_t(memory[address + 48]);
}

Memory::Memory(std::string path)
{
    for (int i = 0; i < 1 << 16; i++) {
        memory[i] = 0;
    }
    std::ifstream input_file(path);
    std::string data = "";
    uint16_t current_address = 0x0;

    while (input_file >> data) {
        if (data[0] == '#') {
            current_address = std::stoi(data.c_str() + 1, 0, 16);
        } else {
            memory[current_address] = std::stoi(data, 0, 16);
            current_address += 16;
        }
    }
}
