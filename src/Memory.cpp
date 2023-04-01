//
// Created by kime on 01.04.2023.
//

#include "Memory.h"
#include <cstring>
#include <fstream>
#include <iostream>

uint64_t Memory::read(uint16_t address)
{
    address /= 2;

    return uint64_t(memory[address]) << 48 | uint64_t(memory[address + 1]) << 32 | uint64_t(memory[address + 2]) << 16 | uint64_t(memory[address + 3]);
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
            current_address = std::stoi(data.c_str() + 1, nullptr, 16);
        } else {
            write(current_address, std::stoi(data, nullptr, 16), 1);
            current_address += 2;
        }
    }
}

void Memory::write(uint16_t address, uint64_t packet, int n_words)
{
    constexpr uint64_t MASK = UINT16_MAX;

    address /= 2;

    switch (n_words) {
    case 4:
        memory[address + 3] = (packet >> 48) & MASK;
    case 3:
        memory[address + 2] = (packet >> 32) & MASK;
    case 2:
        memory[address + 1] = (packet >> 16) & MASK;
    case 1:
        memory[address] = packet & MASK;
    }
}
