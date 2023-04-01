#pragma once

#include "Memory.h"
#include <array>
#include <cstdint>

class DataCache {
    static constexpr uint16_t CACHE_SIZE = 64;

public:
    DataCache();

    std::uint64_t get_fetch_window(Memory&, uint16_t addr, int& cycle_count);

    std::uint16_t read_word(Memory&, uint16_t addr, int& cycle_count);
    void write_word(Memory&, uint16_t addr, uint16_t word, int& cycle_count);

private:
    void populate_cache(Memory& memory, std::uint16_t starting_addr);

    bool m_has_populated_the_cache { false };
    std::uint16_t m_last_read_address { 0 };
    std::array<std::uint16_t, CACHE_SIZE> m_cache;

    // memory write cache
    bool m_has_populated_write_cache { false };
    std::uint16_t m_last_write_address { 0 };
    std::uint16_t m_packet[4] { 0 };
    int m_packet_idx { 0 };
};
