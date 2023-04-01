#pragma once

#include <cstdint>
#include <string>

enum OperandType : std::uint16_t {
    None = 0,
    R0 = 0b0'0001,
    R1 = 0b0'0010,
    R2 = 0b0'0011,
    R3 = 0b0'0100,
    R4 = 0b0'0101,
    R5 = 0b0'0110,
    R6 = 0b0'0111,
    R7 = 0b0'1000,
    Immediate = 0b1'0000,
    Address = 0b1'0001,
    AddressInRegister = 0b1'0010,
};

bool is_register_operand(OperandType);

struct Operand {
    OperandType type { OperandType::None };
    /// Can be an immediate value (address or simple value) or the encoding of a register
    std::uint16_t value;
};

enum class Opcode {
    Add = 0b00'0001,
    Sub = 0b00'0010,
    Mov = 0b00'0011,
    Mul = 0b00'0100,
    Div = 0b00'0101,
    Cmp = 0b00'0110,
    Jmp = 0b00'0111,
    Je = 0b00'1001,
    Jl = 0b00'01010,
    Jg = 0b00'1011,
    Jz = 0b00'1100,
    Call = 0b00'1101,
    Ret = 0b00'1110,
    EndSim = 0b00'1111,
    Push = 0b01'0000,
    Pop = 0b01'0001,
};

struct InstructionData {
    Opcode opcode;
    Operand src1 { OperandType::None };
    Operand src2 { OperandType::None };
    std::string to_string();
};

// FIXME: /// Returns the number of cycles this instruction took to execute
// FIXME: using Instruction = int(*)(InstructionData, Memory&);

/// Returns the size of the instruction from the header
[[nodiscard]] std::size_t instruction_size(std::uint16_t header);

InstructionData decode_instruction(std::uint16_t header, std::uint16_t src1, std::uint16_t src2);