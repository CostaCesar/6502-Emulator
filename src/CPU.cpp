#include "CPU.h"

void CPU::Reset(Memory& memory)
{
    ProgramCounter = 0xFFFC;
    StackPointer = 0x00FF;
    F_Carry = F_Decimal = F_Zero = F_Interupt = F_Break = F_OverFlow = F_Negative = 0;
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
Instruction CPU::FetchInstruction(uint32_t& cycles, const Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return (Instruction) data;
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
    F_Zero = (cpu_register == 0);
    F_Negative = (cpu_register & 0b10000000) > 0;
}
void CPU::LD_SetRegister(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory)
{
    cpu_register = ReadByte(cycles, address, memory);
    LD_SetStatus(cpu_register);
}

/* Increment processor register by value, using up 1 cycle for it */
void CPU::IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}

/* Handles the additional cycle when a load instruction crosses the page border */
void CPU::Check_PageCross(uint32_t& cycles, Word& adrress, Byte offset)
{
    if((int) (adrress / 0xFF) < (int) (adrress + offset) / 0xFF)
        cycles++;
    adrress += offset;
}

uint32_t CPU::Execute(uint32_t cycles_total, Memory& memory)
{
    uint32_t cycles_ran = 0;
    for(cycles_ran; cycles_ran < cycles_total;)
    {
        Instruction instruction = FetchInstruction(cycles_ran, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case INS_LDA_IM:
            RegA = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegA);
            break;
        case INS_LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case INS_LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case INS_LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            LD_SetStatus(RegA);
            break;
        case INS_LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegX);
            break;
        case INS_LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case INS_LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case INS_LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case INS_LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case INS_LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegY);
            break;
        case INS_LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case INS_LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case INS_LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
            break;
        case INS_LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
            break;
        case INS_JSR:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteWord(StackPointer, ProgramCounter-1, cycles_ran);
            ProgramCounter = word_Value;
            cycles_ran++, StackPointer++;
            break;
        default:
            printf("Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}