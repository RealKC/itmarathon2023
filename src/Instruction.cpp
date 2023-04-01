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

bool is_register_operand(OperandType type)
{
    switch (type) {
    case None:
    case Immediate:
    case Address:
        return false;
    default:
        return true;
    }
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

    std::uint16_t insn_value_src1 = 0, insn_value_src2 = 0;

    if (!is_register_operand(static_cast<OperandType>(src1))) {
        insn_value_src1 = src1_value;
    }

    if (!is_register_operand(static_cast<OperandType>(src2))) {
        if (is_register_operand(static_cast<OperandType>(src1))) {
            insn_value_src2 = src1_value;
        } else {
            insn_value_src2 = src2_value;
        }
    }

    return InstructionData {
        .opcode = static_cast<Opcode>(opcode),
        .src1 = Operand {
            .type = static_cast<OperandType>(src1),
            .value = insn_value_src1,
        },
        .src2 = Operand {
            .type = static_cast<OperandType>(src2),
            .value = insn_value_src2,
        }
    };
}

std::string register_to_string(Operand o)
{
    switch (o.type) {
    case OperandType::R0:
        return "R0";
    case OperandType::R1:
        return "R1";
    case OperandType::R2:
        return "R2";
    case OperandType::R3:
        return "R3";
    case OperandType::R4:
        return "R4";
    case OperandType::R5:
        return "R5";
    case OperandType::R6:
        return "R6";
    case OperandType::R7:
        return "R7";
    }
}

std::string InstructionData::to_string()
{
    std::string str = "";
    switch (opcode) {
    case Opcode::Add:
        str += "add";
        break;
    case Opcode::Sub:
        str += "sub";
        break;
    case Opcode::Mov:
        str += "mov";
        break;
    case Opcode::Mul:
        str += "mul";
        break;
    case Opcode::Div:
        str += "div";
        break;
    case Opcode::Cmp:
        str += "cmp";
        break;
    case Opcode::Jmp:
        str += "jmp";
        break;
    case Opcode::Je:
        str += "je";
        break;
    case Opcode::Jl:
        str += "jl";
        break;
    case Opcode::Jg:
        str += "jg";
        break;
    case Opcode::Jz:
        str += "jz";
        break;
    case Opcode::Call:
        str += "call";
        break;
    case Opcode::Ret:
        str += "ret";
        break;
    case Opcode::EndSim:
        str += "end_sim";
        break;
    case Opcode::Push:
        str += "push";
        break;
    case Opcode::Pop:
        str += "sub";
        break;
    }

    if (this->src1.type >= 1 && this->src1.type <= 8) {
        str += " " + register_to_string(src1);
    } else if (this->src1.type == OperandType::Address) {
        str += " [" + std::to_string(src1.value) + "]";
    } else if (this->src1.type == OperandType::AddressInRegister) {
        str += " [" + std::to_string(src1.value) + "]";
    } else if (this->src1.type == OperandType::Immediate) {
        str += " " + std::to_string(src1.value);
    }

    if (this->src2.type >= 1 && this->src2.type <= 8) {
        str += " " + register_to_string(src2);
    } else if (this->src2.type == OperandType::Address) {
        str += " [" + std::to_string(src2.value) + "]";
    } else if (this->src2.type == OperandType::AddressInRegister) {
        str += " [" + std::to_string(src2.value) + "]";
    } else if (this->src2.type == OperandType::Immediate) {
        str += " " + std::to_string(src2.value);
    }
    return str;
}
