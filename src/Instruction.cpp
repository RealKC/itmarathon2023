#include "Instruction.h"

#include <cassert>

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
