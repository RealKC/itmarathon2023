#include "Instruction.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

[[nodiscard]] std::size_t instruction_size(std::uint16_t header)
{
    auto opcode = (header & 0b1111'1100'0000'0000) >> 10;

    switch (static_cast<Opcode>(opcode)) {
    case Opcode::Add:
    case Opcode::Sub:
    case Opcode::Mov:
    case Opcode::Mul:
    case Opcode::Div:
    case Opcode::Cmp:
        return 3;
    case Opcode::Jmp:
    case Opcode::Je:
    case Opcode::Jl:
    case Opcode::Jg:
    case Opcode::Jz:
    case Opcode::Call:
        return 2;
    case Opcode::Ret:
    case Opcode::EndSim:
        return 1;
    case Opcode::Push:
        return 2;
    case Opcode::Pop:
        return 1;
    }

    std::cerr << "[DECODER ERROR] You passed a bad header: " << std::hex << header << std::hex << ", which does not contain a valid opcode." << std::endl;
    std::exit(2);
}

InstructionData decode_instruction(std::uint16_t header, std::uint16_t src1_value, std::uint16_t src2_value)
{
    // clang-format off
    auto opcode = (header & 0b1111'1100'0000'0000) >> 10;
    auto src1   = (header & 0b0000'0011'1110'0000) >> 5;
    auto src2   = (header & 0b0000'0000'0001'1111);
    // clang-format on

    if (src1 == OperandType::None) {
        assert(src2 == OperandType::None && "Incorrect instruction encoding");
    }

    return InstructionData {
        .opcode = static_cast<Opcode>(opcode),
        .src1 = Operand {
            .type = static_cast<OperandType>(src1),
            .value = static_cast<uint16_t>(src1 != OperandType::None ? src1_value : 0),
        },
        .src2 = Operand {
            .type = static_cast<OperandType>(src2),
            .value = static_cast<uint16_t>(src2 != OperandType::None ? src2_value : 0),
        }
    };
}
