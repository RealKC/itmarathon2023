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
    uint64_t read(uint16_t address);
    Memory(std::string path);
};

#endif // ITMARATHON2023_MEMORY_H
