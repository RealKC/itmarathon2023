//
// Created by kime on 01.04.2023.
//

#include "Execute.h"

#include <cassert>
#include <cstring>
#include <iostream>

Execute::Execute(std::uint16_t stack_base, std::uint16_t stack_size)
    : m_stack_base(stack_base)
    , m_stack_size(stack_size)
{
    reset();
}

void Execute::reset()
{
    memset(registers.data(), 0, registers.size() * sizeof(registers[0]));
    sp = m_stack_base;
    flags = 0;
}

void Execute::set_z(bool val)
{
    if (val) {
        flags |= 1;
    } else {
        flags &= ~1u;
    }
}

void Execute::set_e(bool val)
{
    if (val) {
        flags |= 2;
    } else {
        flags &= ~2u;
    }
}

void Execute::set_g(bool val)
{
    if (val) {
        flags |= 4;
    } else {
        flags &= ~4u;
    }
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
            set_z(reg(data.src1) == 0);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                reg(data.src1) += load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                reg(data.src1) += load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                reg(data.src1) += data.src2.value;
                break;
            }
            set_z(reg(data.src1) == 0);
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            load_store.write_word(memory, data.src1.value, left + registers[data.src2.type], cycle_count);
            int tmp;
            set_z(load_store.read_word(memory, data.src1.value, tmp) == 0);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                load_store.write_word(memory, data.src1.value, left + load_store.read_word(memory, reg(data.src2), cycle_count), cycle_count);
                break;
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, left + load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
                break;
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, left + data.src2.value, cycle_count);
                break;
            }
            int tmp;
            set_z(load_store.read_word(memory, data.src1.value, tmp) == 0);
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
            set_z(reg(data.src1) == 0);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                reg(data.src1) -= load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                reg(data.src1) -= load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                reg(data.src1) -= data.src2.value;
                break;
            }
            set_z(reg(data.src1) == 0);
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            load_store.write_word(memory, data.src1.value, left - registers[data.src2.type], cycle_count);
            int tmp;
            set_z(load_store.read_word(memory, data.src1.value, tmp) == 0);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                load_store.write_word(memory, data.src1.value, left - load_store.read_word(memory, reg(data.src2), cycle_count), cycle_count);
                break;
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, left - load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
                break;
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, left - data.src2.value, cycle_count);
                break;
            }
            int tmp;
            set_z(load_store.read_word(memory, data.src1.value, tmp) == 0);
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
                break;
            case OperandType::Address:
                reg(data.src1) = load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                reg(data.src1) = data.src2.value;
                break;
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
                break;
            case OperandType::Address:
                load_store.write_word(memory, data.src1.value, load_store.read_word(memory, data.src2.value, cycle_count), cycle_count);
                break;
            case OperandType::Immediate:
                load_store.write_word(memory, data.src1.value, data.src2.value, cycle_count);
                break;
            }
            return cycle_count;
        }
    }
}

int Execute::mul(InstructionData data, DataCache& load_store, Memory& memory)
{
    uint64_t result = 0;
    int cycle_count = 5;
    if (is_register_operand(data.src1.type)) {
        if (is_register_operand(data.src2.type)) {
            result = reg(data.src1) * reg(data.src2);
            set_z(result == 0);
            registers[0] = result >> 16;
            registers[1] = result & 0xffff;
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                result = reg(data.src1) * load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                result = reg(data.src1) * load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                result = reg(data.src1) * data.src2.value;
                break;
            }
            set_z(result == 0);
            registers[0] = result >> 16;
            registers[1] = result & 0xffff;
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            result = left * registers[data.src2.type];
            set_z(result == 0);
            registers[0] = result >> 16;
            registers[1] = result & 0xffff;
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                result = left * load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                result = left * load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                result = left * data.src2.value;
                break;
            }
            set_z(result == 0);
            registers[0] = result >> 16;
            registers[1] = result & 0xffff;
            return cycle_count;
        }
    }
}

int Execute::div(InstructionData data, DataCache& load_store, Memory& memory)
{

    int cycle_count = 5;
    if (is_register_operand(data.src1.type)) {
        if (is_register_operand(data.src2.type)) {
            registers[0] = reg(data.src1) / reg(data.src2);
            registers[1] = reg(data.src1) % reg(data.src2);
            set_z(registers[0] == 0);

            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                registers[0] = reg(data.src1) / load_store.read_word(memory, reg(data.src2), cycle_count);
                registers[1] = reg(data.src1) % load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                registers[0] = reg(data.src1) / load_store.read_word(memory, data.src2.value, cycle_count);
                registers[1] = reg(data.src1) % load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                registers[0] = reg(data.src1) / data.src2.value;
                registers[1] = reg(data.src1) % data.src2.value;
                break;
            }
            set_z(registers[0] == 0);
            return cycle_count;
        }
    } else {
        auto left = load_store.read_word(memory, data.src1.value, cycle_count);

        if (is_register_operand(data.src2.type)) {
            registers[0] = left / registers[data.src2.type];
            registers[1] = left % registers[data.src2.type];
            set_z(registers[0] == 0);
            return cycle_count;
        } else {
            switch (data.src2.type) {
            case OperandType::AddressInRegister:
                registers[0] = left / load_store.read_word(memory, reg(data.src2), cycle_count);
                registers[1] = left % load_store.read_word(memory, reg(data.src2), cycle_count);
                break;
            case OperandType::Address:
                registers[0] = left / load_store.read_word(memory, data.src2.value, cycle_count);
                registers[1] = left % load_store.read_word(memory, data.src2.value, cycle_count);
                break;
            case OperandType::Immediate:
                registers[0] = left / data.src2.value;
                registers[1] = left % data.src2.value;
                break;
            }
            set_z(registers[0] == 0);
            return cycle_count;
        }
    }
}

int Execute::cmp(InstructionData data, DataCache& load_store, Memory& memory)
{
    uint16_t val1, val2;
    if (is_register_operand(data.src1.type))
        val1 = reg(data.src1);
    else
        val1 = data.src1.value;
    if (is_register_operand(data.src2.type))
        val2 = reg(data.src2);
    else
        val2 = data.src2.value;
    set_z(val1 == val2 == 0);
    set_e(val1 == val2);
    set_g(val1 > val2);
    return 5;
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
