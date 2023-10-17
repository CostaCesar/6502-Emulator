#include "CPU.h"
using namespace Instruction;

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
void CPU::LD_SetStatus(Byte& cpu_register)
{
    Flags.Zero = (cpu_register == 0);
    Flags.Negative = (cpu_register & 0b10000000) > 0;
}
void CPU::LD_SetRegister(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory)
{
    cpu_register = ReadByte(cycles, address, memory);
    LD_SetStatus(cpu_register);
}

Byte CPU::PopByte_Stack(uint32_t& cycles, const Memory& memory)
{
    Byte output = ReadByte(cycles, StackPointer_ToWord() + 1, memory);
    StackPointer++;
    cycles += 2;
    return output;
}

void CPU::PushByte_Stack(uint32_t& cycles, Byte value, Memory& memory)
{
    memory.WriteByte(StackPointer_ToWord(), value, cycles);
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
    memory.WriteWord(StackPointer_ToWord() - 1, ProgramCounter, cycles);
    cycles++;
    StackPointer -= 2;
}
/* Get previous Program Counter from the Stack*/
Word CPU::PopWord_Stack(uint32_t& cycles, Memory& memory)
{
    Word output = ReadWord(cycles, StackPointer_ToWord() + 1, memory);
    cycles += 3;
    StackPointer += 2;
    return output;
}

/* Convert the Stack Pointer's current address to a Word value*/
Word CPU::StackPointer_ToWord() const
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
            LD_SetStatus(RegA);
            break;
        case LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegX);
            break;
        case LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegY);
            break;
        case LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
            break;
        case LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
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
            // Cycle used to secure memory in case of overflow in the addition of the adrres
            cycles_ran++;   
            break;
        case STA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            memory.WriteByte(word_Value + RegY, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the adrres   
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
            LD_SetStatus(RegX);
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
            LD_SetStatus(RegA);
            break; 
        case PLP:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            break;
        case AND_IM:
            break;
        case AND_ZP:
            break; 
        case AND_ZPX:
            break;
        case AND_AB:
            break;
        case AND_ABX:
            break;
        case AND_ABY:
            break; 
        case AND_IDX:
            break; 
        case AND_IDY:
            break;
        case ORA_IM:
            break;
        case ORA_ZP:
            break;
        case ORA_ZPX:
            break;
        case ORA_AB:
            break;
        case ORA_ABX:
            break;
        case ORA_ABY:
            break;
        case ORA_IDX:
            break;
        case ORA_IDY:
            break;
        case EOR_IM:
            break;
        case EOR_ZP:
            break;
        case EOR_ZPX:
            break;
        case EOR_AB:
            break;
        case EOR_ABX:
            break;
        case EOR_ABY:
            break;
        case EOR_IDX:
            break;
        case EOR_IDY:
            break;
        default:
            printf("Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}