//
// Created by kime on 01.04.2023.
//

#include "Execute.h"

#include <cassert>
#include <cstring>
#include <iostream>

Execute::Execute()
{
    reset();
}

void Execute::reset()
{
    memset(registers.data(), 0, registers.size() * sizeof(registers[0]));
    sp = 0;
    flags = 0;
}

uint16_t& Execute::reg(Operand operand)
{
    assert(OperandType::R0 <= operand.type && operand.type <= OperandType::R7);

    return registers[static_cast<std::size_t>(operand.type - 1)];
}

int Execute::add(InstructionData data, DataCache& load_store, Memory& memory)
{
    int cycle_count = 5;
    if (is_register_operand(data.src1.type)) {
        if (is_register_operand(data.src2.type)) {
            reg(data.src1) += reg(data.src2);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                reg(data.src1) += load_store.read_word(memory, reg(data.src2), cycle_count);
            case OperandType::Address:
                reg(data.src1) += load_store.read_word(memory, data.src2.value, cycle_count);
            case OperandType::Immediate:
                reg(data.src1) += data.src2.value;
            }
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            load_store.write_word(memory, data.src1.value, left + registers[data.src2.type], cycle_count);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                load_store.write_word(memory, data.src1.value, left + load_store.read_word(memory, reg(data.src2), cycle_count), cycle_count);
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, left + load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, left + data.src2.value, cycle_count);
            }
            return cycle_count;
        }
    }
    return 0;
}

int Execute::sub(InstructionData data, DataCache& load_store, Memory& memory)
{
    int cycle_count = 5;
    if (is_register_operand(data.src1.type)) {
        if (is_register_operand(data.src2.type)) {
            reg(data.src1) -= reg(data.src2);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                reg(data.src1) -= load_store.read_word(memory, reg(data.src2), cycle_count);
            case OperandType::Address:
                reg(data.src1) -= load_store.read_word(memory, data.src2.value, cycle_count);
            case OperandType::Immediate:
                reg(data.src1) -= data.src2.value;
            }
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            load_store.write_word(memory, data.src1.value, left - registers[data.src2.type], cycle_count);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                load_store.write_word(memory, data.src1.value, left - load_store.read_word(memory, reg(data.src2), cycle_count), cycle_count);
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, left - load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, left - data.src2.value, cycle_count);
            }
            return cycle_count;
        }
    }
}

int Execute::mov(InstructionData data, DataCache& load_store, Memory& memory)
{
    int cycle_count = 5;
    if (is_register_operand(data.src1.type)) {
        if (is_register_operand(data.src2.type)) {
            reg(data.src1) = reg(data.src2);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                reg(data.src1) = load_store.read_word(memory, reg(data.src2), cycle_count);
            case OperandType::Address:
                reg(data.src1) = load_store.read_word(memory, data.src2.value, cycle_count);
            case OperandType::Immediate:
                reg(data.src1) = data.src2.value;
            }
            return cycle_count;
        }
    } else {
        if (is_register_operand(data.src2.type)) {
            load_store.write_word(memory, data.src1.value, registers[data.src2.type], cycle_count);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                load_store.write_word(memory, data.src1.value, load_store.read_word(memory, reg(data.src2), cycle_count), cycle_count);
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, data.src2.value, cycle_count);
            }
            return cycle_count;
        }
    }
}

int Execute::mul(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::div(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::cmp(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::jmp(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::je(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::jl(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::jg(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::jz(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::call(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::ret(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::end_sim(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::push(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}

int Execute::pop(InstructionData data, DataCache& load_store, Memory& memory)
{
    return 0;
}
