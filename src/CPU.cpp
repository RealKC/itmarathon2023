#include "CPU.h"

#include "Instruction.h"
#include <iostream>

CPU::CPU()
    : EX(0x7000, 0x512)
{
    reset();
}

void CPU::reset()
{
    IP = 0xfff0;
    EX.reset();
    LS.reset();
    IC.reset();
}

void CPU::run_cycles(Memory& memory)
{
    bool done = false;
    while (!done) {
        int current_insn_cycles;
        RawInstruction next_insn = IC.next_instruction(IP, LS, memory, current_insn_cycles);
        IP += next_insn.size * 2;
        InstructionData decoded_insn = decode_instruction(next_insn.header, next_insn.src1, next_insn.src2);
        std::cout << "Executing: " << decoded_insn.to_string() << std::endl;
        EX.dispatch(decoded_insn, LS, memory, IP, done);
    }
}
