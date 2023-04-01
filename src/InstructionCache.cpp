#include "InstructionCache.h"

#include "Instruction.h"
#include <cassert>
#include <iostream>

InstructionCache::InstructionCache()
{
    reset();
}

void InstructionCache::reset()
{
    m_fetched = 0;
    m_last_ip = 0;
    m_has_populated_fetched = false;
}

RawInstruction InstructionCache::next_instruction(const std::uint16_t ip, DataCache& ls, Memory& mem, int& cycle_count)
{
    auto fetched = ls.get_fetch_window(mem, ip, cycle_count);
    //    std::cout << "fetched: " << std::hex << fetched << std::dec << std::endl;
    return RawInstruction {
        .size = static_cast<int>(instruction_size(fetched >> 48)),
        .header = static_cast<uint16_t>(fetched >> 48),
        .src1 = static_cast<uint16_t>(fetched >> 32),
        .src2 = static_cast<uint16_t>(fetched >> 16),
    };

    //    if (!m_has_populated_fetched) {
    //        m_has_populated_fetched = true;
    //        m_fetched = ls.get_fetch_window(mem, ip, cycle_count);
    //        m_last_ip = ip;
    //    }
    //
    //    // Simple case: IP is an address that's not been cached at all
    //    if (!(m_last_ip <= ip && ip <= m_last_ip + 8)) {
    //        m_fetched = ls.get_fetch_window(mem, ip, cycle_count);
    //        m_last_ip = ip;
    //    }
    //
    //    // At this point, IP points to either a fully or partially cached instruction
    //
    //    auto idx = (ip % m_last_ip) / 2;
    //    int shift_by = 0;
    //    switch (idx) {
    //    case 0:
    //        shift_by = 48;
    //        break;
    //    case 1:
    //        shift_by = 32;
    //        break;
    //    case 2:
    //        shift_by = 16;
    //        break;
    //    case 3:
    //        shift_by = 0;
    //        break;
    //    }
    //
    //    std::uint16_t header = m_fetched >> shift_by;
    //
    //    auto insn_size = instruction_size(header);
    //    //    std::cout << "\tsize of insn: " << insn_size << std::endl;
    //
    //    if ((m_last_ip + idx + insn_size) <= (m_last_ip + 4)) {
    //        // Simple case: the instruction is fully cached!
    //        switch (insn_size) {
    //        case 1:
    //            return RawInstruction {
    //                .size = 1,
    //                .header = header,
    //            };
    //        case 2:
    //            return RawInstruction {
    //                .size = 2,
    //                .header = header,
    //                .src1 = static_cast<uint16_t>(m_fetched >> (shift_by - 16)),
    //            };
    //        case 3:
    //            return RawInstruction {
    //                .size = 3,
    //                .header = header,
    //                .src1 = static_cast<uint16_t>(m_fetched >> (shift_by - 16)),
    //                .src2 = static_cast<uint16_t>(m_fetched >> (shift_by - 32)),
    //            };
    //        default:
    //            std::cerr << "[INTERNAL IC ERROR] Bad instruction size: " << insn_size << std::endl;
    //            assert(false);
    //        }
    //    }
    //
    //    // Worst case: instruction is partially cached
    //    // We have two cases to handle:
    //    //      1. cache: [ ..., header, src1 ], memory: [ src2 ]
    //    //      2. cache: [ ..., header ], memory: [ src1, src2 ]
    //    //      3. cache: [ ..., header ], memory: [ src1 ]
    //    // #1 and #3 will have the same not cached count
    //
    //    auto not_cached_count = 4 - idx - insn_size;
    //
    //    RawInstruction insn;
    //
    //    switch (not_cached_count) {
    //    case 1: {
    //        auto new_fetch_window = ls.get_fetch_window(mem, ip + 8, cycle_count);
    //        if (insn_size == 3) {
    //            // Case #1 from above
    //            insn.size = 3;
    //            insn.header = m_fetched >> shift_by;
    //            insn.src1 = m_fetched >> (shift_by - 16);
    //            insn.src2 = new_fetch_window >> 48;
    //        } else if (insn_size == 2) {
    //            // Case #3 from above
    //            insn.size = 2;
    //            insn.header = m_fetched >> shift_by;
    //            insn.src1 = new_fetch_window >> 48;
    //        }
    //        m_fetched = new_fetch_window;
    //        m_last_ip = ip;
    //        return insn;
    //    }
    //    case 2: {
    //        auto new_fetch_window = ls.get_fetch_window(mem, ip + 2, cycle_count);
    //
    //        insn.size = 3;
    //        insn.header = m_fetched >> shift_by;
    //        insn.src1 = new_fetch_window >> 48;
    //        insn.src2 = new_fetch_window >> 32;
    //
    //        m_fetched = new_fetch_window;
    //        m_last_ip = ip;
    //
    //        return insn;
    //    }
    //    default:
    //        std::cerr << "[INTERNAL IC ERROR] Bad not_cached_count: " << not_cached_count << std::endl;
    //        assert(false);
    //    }
}
