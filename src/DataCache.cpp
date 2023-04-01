#include "DataCache.h"

#include <cstring>
#include <iostream>

DataCache::DataCache()
{
    memset(m_cache.data(), 0, m_cache.size() * sizeof(uint16_t));
}

std::uint64_t DataCache::get_fetch_window(Memory& memory, uint16_t addr, int& cycle_count)
{
    if (m_last_read_address <= addr && addr <= m_last_read_address + CACHE_SIZE * 2 && m_has_populated_the_cache) {
        cycle_count += 7;
    } else {
        // If we get a cache miss, we just repopulate the entire cache
        // There probably are smarter strategies, but I'm not sure yet.
        populate_cache(memory, addr);
        cycle_count += 15;
    }

    auto idx = (addr % m_last_read_address) / 2;
    return static_cast<uint64_t>(m_cache[idx]) << 48
        | static_cast<uint64_t>(m_cache[idx + 1]) << 32
        | static_cast<uint64_t>(m_cache[idx + 2]) << 16
        | static_cast<uint64_t>(m_cache[idx + 3]);
}

void DataCache::populate_cache(Memory& memory, std::uint16_t starting_addr)
{
    m_has_populated_the_cache = true;
    m_last_read_address = starting_addr;

    for (auto offset = 0; offset <= CACHE_SIZE; offset += 4) {
        auto packet = memory.read(starting_addr + offset * 2);
        m_cache[offset] = packet >> 48;
        m_cache[offset + 1] = packet >> 32;
        m_cache[offset + 2] = packet >> 16;
        m_cache[offset + 3] = packet;
    }
}

std::uint16_t DataCache::read_word(Memory& memory, uint16_t addr, int& cycle_count)
{
    if ((addr & 1) == 1) {
        // FIXME: implement exception for misaligned access
        return 0;
    }

    if (m_last_read_address <= addr && addr <= m_last_read_address + CACHE_SIZE && m_has_populated_the_cache) {
        cycle_count += 7;
    } else {
        // If we get a cache miss, we just repopulate the entire cache
        // There probably are smarter strategies, but I'm not sure yet.
        populate_cache(memory, addr);
        cycle_count += 15;
    }

    auto idx = addr % m_last_read_address;
    return static_cast<uint64_t>(m_cache[idx]);
}

void DataCache::write_word(Memory& memory, uint16_t addr, uint16_t word, int& cycle_count)
{
    if ((addr & 1) == 1) {
        // FIXME: Notify about exception
        return;
    }

    cycle_count += 15;

    // If the write is in cached memory, update the cache
    if (m_last_read_address <= addr && addr <= m_last_read_address + CACHE_SIZE && m_has_populated_the_cache) {
        m_cache[addr % m_last_read_address] = word;
    }

    if (m_last_write_address <= addr && addr < m_last_write_address + 4 && m_has_populated_the_cache) {
        auto idx = ((addr - 1) % m_last_write_address);
        if (idx <= m_packet_idx) {
            m_packet[idx] = word;
        } else {
            m_packet_idx = idx;
            m_packet[idx] = word;
        }
    } else {
        m_has_populated_write_cache = true;
        uint64_t packet = static_cast<uint64_t>(m_packet[3]) << 48
            | static_cast<uint64_t>(m_packet[2]) << 32
            | static_cast<uint64_t>(m_packet[1]) << 16
            | static_cast<uint64_t>(m_packet[0]);
        memory.write(m_last_write_address, packet, m_packet_idx + 1);
        m_last_write_address = addr;
        m_packet_idx = 0;
        m_packet[m_packet_idx] = word;
    }
}
