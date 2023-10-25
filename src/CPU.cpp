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
void CPU::Shift_Value_Carring(uint32_t& cycles, Byte& value, char direc)
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
    cycles += 2;
    return output;
}

void CPU::PushByte_Stack(uint32_t& cycles, Byte value, Memory& memory)
{
    memory.WriteByte(Stack_AsWord(), value, cycles);
    StackPointer--;
    cycles++;
    return;
}

/* Increment data by value from register, using up 1 cycle for it */
void CPU::IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}
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
void CPU::PushWord_Stack(uint32_t& cycles, Memory& memory)
{
    memory.WriteWord(Stack_AsWord() - 1, ProgramCounter, cycles);
    cycles++;
    StackPointer -= 2;
}
/* Get previous Program Counter from the Stack*/
Word CPU::PopWord_Stack(uint32_t& cycles, Memory& memory)
{
    Word output = ReadWord(cycles, Stack_AsWord() + 1, memory);
    cycles += 3;
    StackPointer += 2;
    return output;
}

/* Convert the Stack Pointer's current address to a Word value*/
Word CPU::Stack_AsWord() const
{ return 0x0100 | StackPointer; }

uint32_t CPU::Execute(uint32_t cycles_total, Memory& memory)
{
    uint32_t cycles_ran = 0;
    for(cycles_ran; cycles_ran < cycles_total;)
    {
        Opcode instruction = FetchInstruction(cycles_ran, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case LDA_IM:
            RegA = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            break;
        case LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegX);
            break;
        case LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegY);
            break;
        case LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case STA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case STA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case STA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegY);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value + RegX, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the address
            cycles_ran++;   
            break;
        case STA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            memory.WriteByte(word_Value + RegY, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the address   
            cycles_ran++;
            break;
        case STX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break;  
        case STX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break; 
        case STX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegX, cycles_ran);
            break; 
        case STY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break;  
        case STY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break; 
        case STY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegY, cycles_ran);
            break; 
        case JSR:
            word_Value = FetchWord(cycles_ran, memory);
            PushWord_Stack(cycles_ran, memory);
            ProgramCounter = word_Value;
            break;
        case RTS:
            ProgramCounter = PopWord_Stack(cycles_ran, memory);
            break;
        case JMP_AB:
            word_Value = FetchWord(cycles_ran, memory);
            ProgramCounter = word_Value;
            break;
        case JMP_ID:
            word_Value = FetchWord(cycles_ran, memory);
            if(ChipModel == CHIP_STANDART && (word_Value + 1) % 0x0100 == 0)
            {
                byte_Value = ReadByte(cycles_ran, ((int) word_Value - 0x00FF), memory);
                word_Value = (byte_Value << 8) + ReadByte(cycles_ran, word_Value, memory);
            }
            else word_Value = ReadWord(cycles_ran, word_Value, memory);
            ProgramCounter = word_Value;
            break;
        case TSX:
            RegX = StackPointer;
            cycles_ran++;
            SetStatus_NegvZero(RegX);
            break;
        case TXS:
            StackPointer = RegX;
            cycles_ran++;
            break;    
        case PHA:
            PushByte_Stack(cycles_ran, RegA, memory);
            break;     
        case PHP:
            PushByte_Stack(cycles_ran, FlagStatus, memory);
            break;    
        case PLA:
            RegA = PopByte_Stack(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            break; 
        case PLP:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            break;
        case AND_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA &= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case AND_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break; 
        case AND_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break;
        case AND_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break; 
        case AND_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
         case ORA_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA |= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case ORA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break; 
        case ORA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break;
        case ORA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break; 
        case ORA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
         case EOR_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA ^= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case EOR_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break; 
        case EOR_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break;
        case EOR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break; 
        case EOR_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case BIT_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case BIT_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case DEC_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case DEC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = (Word) byte_Value;
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case DEC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case DEC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case INC_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case INC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = (Word) byte_Value;
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case INC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case INC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case ASL_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '<');
            break;
        case ASL_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ASL_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case ASL_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ASL_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '>');
            break;
        case LSR_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = (Word) byte_Value;
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case LSR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROL_RGA:
            Shift_Value_Carring(cycles_ran, RegA, '<');
            break;
        case ROR_RGA:
            break;
        default:
            printf("Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}