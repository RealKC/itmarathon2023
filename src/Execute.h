//
// Created by kime on 01.04.2023.
//

#pragma once

#include "DataCache.h"
#include "Instruction.h"
#include "Memory.h"

class Execute {
public:
    Execute(std::uint16_t stack_base, std::uint16_t stack_size);

    void reset();

    int add(InstructionData, DataCache&, Memory&);
    int sub(InstructionData, DataCache&, Memory&);
    int mov(InstructionData, DataCache&, Memory&);
    int mul(InstructionData, DataCache&, Memory&);
    int div(InstructionData, DataCache&, Memory&);
    int cmp(InstructionData, DataCache&, Memory&);
    int jmp(InstructionData, DataCache&, Memory&);
    int je(InstructionData, DataCache&, Memory&);
    int jl(InstructionData, DataCache&, Memory&);
    int jg(InstructionData, DataCache&, Memory&);
    int jz(InstructionData, DataCache&, Memory&);
    int call(InstructionData, DataCache&, Memory&);
    int ret(InstructionData, DataCache&, Memory&);
    int end_sim(InstructionData, DataCache&, Memory&);
    int push(InstructionData, DataCache&, Memory&);
    int pop(InstructionData, DataCache&, Memory&);

private:
    uint16_t& reg(Operand);

    void set_e(bool val);
    void set_g(bool val);
    void set_z(bool val);

    std::array<uint16_t, 8> registers { 0 };
    uint16_t sp { 0 }, flags { 0 };

    std::uint16_t m_stack_base;
    std::uint16_t m_stack_size;
};
