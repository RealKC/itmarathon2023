#pragma once

#include "DataCache.h"
#include "Execute.h"
#include "InstructionCache.h"
#include "Memory.h"

class CPU {
public:
    CPU();

    void reset();

    void run_cycles(Memory&);

private:
    std::uint16_t IP;

    InstructionCache IC;
    Execute EX;
    DataCache LS;

    int m_total_cycles { 0 };
};
