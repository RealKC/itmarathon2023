#pragma once

#include <cstdint>

enum OperandType : std::uint16_t {
    None = 0,
    R0 = 0'0001,
    R1 = 0'0010,
    R2 = 0'0011,
    R3 = 0'0100,
    R4 = 0'0101,
    R5 = 0'0110,
    R6 = 0'0111,
    R7 = 0'1000,
    Immediate = 1'0000,
    Address = 1'0001,
    AddressInRegister = 1'0010,
};

struct Operand {
    OperandType type { OperandType::None };
    /// Can be an immediate value (address or simple value) or the encoding of a register
    std::uint16_t value;
};

enum class Opcode {
    Add = 00'0001,
    Sub = 00'0010,
    Mov = 00'0011,
    Mul = 00'0100,
    Div = 00'0101,
    Cmp = 00'0110,
    Jmp = 00'0111,
    Je = 00'1001,
    Jl = 00'01010,
    Jg = 00'1011,
    Jz = 00'1100,
    Call = 00'1101,
    Ret = 00'1110,
    EndSim = 00'1111,
    Push = 01'0000,
    Pop = 01'0001,
};

struct InstructionData {
    Opcode opcode;
    Operand src1 { OperandType::None };
    Operand src2 { OperandType::None };

    /// Returns the size of this instruction in 16-bit words.
    [[nodiscard]] int size() const {
        int size = 1;

        if (src1.type != OperandType::None) {
            size += 1;
        }

        if (src2.type != OperandType::None) {
            size += 1;
        }

        return size;
    }
};

// FIXME: /// Returns the number of cycles this instruction took to execute
// FIXME: using Instruction = int(*)(InstructionData, Memory&);

InstructionData decode_instruction(std::uint16_t header, std::uint16_t src1, std::uint16_t src2);