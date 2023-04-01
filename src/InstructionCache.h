#pragma once

#include "DataCache.h"
#include "Memory.h"

/// Represents an instruction that has not yet been decoded
struct RawInstruction {
    int size { 0 };
    std::uint16_t header { 0 };
    std::uint16_t src1 { 0 };
    std::uint16_t src2 { 0 };
};

class InstructionCache {
public:
    InstructionCache();

    void reset();

    /// Returns the size of the instruction
    RawInstruction next_instruction(std::uint16_t ip, DataCache& ls, Memory& mem, int& cycle_count);

private:
    /// This field represents the last memory address from which we fetched memory
    /// not necessarily the last ip that we used
    std::uint16_t m_last_ip { 0 };
    bool m_has_populated_fetched { false };
    std::uint64_t m_fetched { 0 };
};
