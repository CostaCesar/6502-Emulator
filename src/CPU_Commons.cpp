#include "CPU.h"
using namespace Instruction;

CPU::CPU(Variant chip_Model, Memory& memory, Word start_from)
{
    ChipModel = chip_Model;
    Reset(start_from, memory);
}

void CPU::Reset(Memory& memory)
{
    Reset(0xFFFC, memory);
}
void CPU::Reset(Word address, Memory& memory)
{
    ProgramCounter = address;
    StackPointer = 0x00FF;
    FlagStatus = 0;
    RegA = RegY = RegX = 0;
    memory.Initialise();
}

/* Read 1 byte from memory */
Byte CPU::ReadByte(uint32_t& cycles, Word address, const Memory& memory)
{
    Byte data = memory[address];
    cycles++;
    return data;
}

/* Read 1 byte from memory AND increment program counter */
Byte CPU::FetchByte(uint32_t& cycles, const Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return data;
}
/* Read instruction from memory AND increment program counter */
Opcode CPU::FetchInstruction(uint32_t& cycles, const Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return (Opcode) data;
}

/* Read 1 word (2 bytes) from memory */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::ReadWord(uint32_t& cycles, Word address, const Memory& memory)
{
    // My platform is little endian
    Word data = memory[address];
    data |= (memory[++address] << 8);
    cycles += 2;
    return data;
}

/* Read 1 word (2 bytes) from memory AND increment program counter */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::FetchWord(uint32_t& cycles, const Memory& memory)
{
    // 6502 is little endian
    Word data = ReadWord(cycles, ProgramCounter, memory);
    ProgramCounter += 2;
    return data;
}

/* Set flags required by a LDA operation */
void CPU::SetStatus_NegvZero(Byte cpu_register)
{
    Flags.Zero = (cpu_register == 0);
    Flags.Negative = (cpu_register & 0b10000000) > 0;
}

void CPU::Load_Register(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory)
{
    cpu_register = ReadByte(cycles, address, memory);
    SetStatus_NegvZero(cpu_register);
}
void CPU::Logical_Operate(uint32_t& cycles, Word address, char operation, const Memory& memory)
{
    Byte value = ReadByte(cycles, address, memory);
    switch (operation)
    {
    case '&': RegA &= value;
        break;
    case '|': RegA |= value;
        break;
    case '^': RegA ^= value;
        break;
    }
    SetStatus_NegvZero(RegA);
}
void CPU::Memory_Increment(uint32_t& cycles, Word address, Byte increment_value, Memory& memory)
{
    Byte byte_Value = ReadByte(cycles, address, memory);
    IncrementByRegister(cycles, byte_Value, increment_value);
    memory.WriteByte(address, byte_Value, cycles);
    SetStatus_NegvZero(byte_Value);
}
void CPU::Bit_Test(uint32_t& cycles, Word address, const Memory& memory)
{
    Byte value = ReadByte(cycles, address, memory);
    Flags.Zero = !(RegA & value);
    FlagStatus |= (value & 0b11000000);
}
void CPU::Shift_Value_WithZero(uint32_t& cycles, Byte& value, char direc)
{
    if(direc == '<')
    {
        Flags.Carry = (value & 0b10000000) != 0;
        value = value << 1;
    }
    else if(direc == '>')
    {
        Flags.Carry = (value & 0b00000001) != 0;
        value = value >> 1;
    }
    cycles++;
    SetStatus_NegvZero(value);
}
void CPU::Shift_Value_Carrying(uint32_t& cycles, Byte& value, char direc)
{
    Byte NewCarry = 0;
    NewCarry = Flags.Carry;
    if(direc == '<')
    {
        Flags.Carry = (value & 0b10000000) != 0;
        value = value << 1;
        value |= NewCarry;
    }
    else if(direc == '>')
    {
        Flags.Carry = (value & 0b00000001) != 0;
        value = value >> 1;
        value |= NewCarry << 7;
    }
    cycles++;
    SetStatus_NegvZero(value);
}

Byte CPU::PopByte_Stack(uint32_t& cycles, const Memory& memory)
{
    Byte output = ReadByte(cycles, Stack_AsWord() + 1, memory);
    StackPointer++;
    cycles++;
    return output;
}
void CPU::PushByte_Stack(uint32_t& cycles, Byte value, Memory& memory)
{
    memory.WriteByte(Stack_AsWord(), value, cycles);
    StackPointer--;
    cycles++;
    return;
}
void CPU::PushWord_Stack(uint32_t& cycles, Word value, Memory& memory)
{
    memory.WriteWord(Stack_AsWord(), value, cycles);
    StackPointer -= 2;
    cycles++;
    return;
}
Word CPU::PopWord_Stack(uint32_t& cycles, const Memory& memory)
{
    Word output = ReadWord(cycles, Stack_AsWord() + 1, memory);
    StackPointer += 2;
    cycles++;
    return output;
}

/* Increment data by value from register, using up 1 cycle for it */
void CPU::IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}
/* Increment data by value from register, using up 1 cycle for it */
void CPU::IncrementByRegister(uint32_t& cycles, Word& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}

/* Handles the additional cycle when a load instruction crosses the page border */
void CPU::Check_PageCross(uint32_t& cycles, Word& address, Byte offset)
{
    if((int) (address / 0xFF) < (int) (address + offset) / 0xFF)
        cycles++;
    address += offset;
}

/* Write current Program Counter (-1) to the Stack*/
void CPU::Push_ProgCount_Stack(uint32_t& cycles, Memory& memory)
{
    memory.WriteWord(Stack_AsWord() - 1, ProgramCounter, cycles);
    StackPointer -= 2;
}
/* Get previous Program Counter from the Stack*/
Word CPU::Pop_PC_Stack(uint32_t& cycles, Memory& memory)
{
    Word output = ReadWord(cycles, Stack_AsWord() + 1, memory);
    cycles += 3;
    StackPointer += 2;
    return output;
}

/* Convert the Stack Pointer's current address to a Word value*/
Word CPU::Stack_AsWord() const
{ return 0x0100 | StackPointer; }

/* Read 1 byte as a WORD from memory AND increment program counter */
Word CPU::FetchByte_AsWord(uint32_t &cycles, const Memory &memory)
{
    return (Word) FetchByte(cycles, memory);
}

void CPU::Math_Add(uint32_t &cycles, Byte value)
{
    Word previous_Value = RegA;
    Word sum = 0;
    
    if(Flags.Decimal == 1)
    {
        RegA = (((RegA & 0b11110000) >> 4) * 10) + (RegA & 0b00001111);
        value = (((value & 0b11110000) >> 4) * 10) + (value & 0b00001111);
    }

    sum = RegA + value + Flags.Carry;
    RegA = (Byte) sum;

    Flags.Carry = Flags.Decimal ? sum > 99 : sum > 0xFF;
    Flags.OverFlow = (~(previous_Value ^ value)) & (previous_Value ^ sum) & 0b10000000; // sum might be RegA, check later
    
    SetStatus_NegvZero(RegA);
}
void CPU::Math_Sub(uint32_t &cycles, Byte value)
{
    Math_Add(cycles, ~value + 1);
}