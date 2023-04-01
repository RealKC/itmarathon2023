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
    int total_cycles;

    bool done = false;
    while (!done) {
        int current_insn_cycles = 0;
        RawInstruction next_insn = IC.next_instruction(IP, LS, memory, current_insn_cycles);
        InstructionData decoded_insn = decode_instruction(next_insn.header, next_insn.src1, next_insn.src2);
        std::cout << "Executing: " << decoded_insn.to_string() << " @ " << IP << "(0x" << std::hex << IP << std::dec << ")" << std::endl;
        IP += next_insn.size * 2;
        EX.dispatch(decoded_insn, LS, memory, IP, done);
        dump_state();
        std::cout << "Executing this instruction took " << current_insn_cycles << " cycles.\n\n";
        total_cycles += current_insn_cycles;
    }

    std::cout << "Simulare a luat sfarsit" << std::endl;
    std::cout << "Executing this program took " << total_cycles << " cycles" << std::endl;
}

void CPU::dump_state()
{
    std::cout << "Registers:" << std::endl;
    std::cout << std::hex << "    IP = " << IP << " SP = " << EX.get_sp() << std::dec << std::endl;
    EX.dump_gprs();
    std::cout << "Flags: " << std::endl;
    EX.dump_flags();
    std::cout << "\n";
}
