//
// Created by kime on 01.04.2023.
//

#ifndef ITMARATHON2023_MEMORY_H
#define ITMARATHON2023_MEMORY_H
#include <array>
#include <cstdint>
#include <string>

class Memory {
private:
    std::array<uint16_t, 1 << 16> memory;

public:
    Memory(std::string path);

    uint64_t read(uint16_t address);
    void write(uint16_t address, uint64_t packet, int n_words);
};

#endif // ITMARATHON2023_MEMORY_H
