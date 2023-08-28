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
Byte CPU::ReadByte(uint32_t& cycles, Word address, Memory& memory)
{
    Byte data = memory[address];
    cycles--;
    return data;
}

/* Read 1 byte from memory AND increment program counter */
Byte CPU::FetchByte(uint32_t& cycles, Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return data;
}
/* Read instruction from memory AND increment program counter */
Instruction CPU::FetchInstruction(uint32_t& cycles, Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return (Instruction) data;
}

/* Read 1 word (2 bytes) from memory */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::ReadWord(uint32_t& cycles, Word address, Memory& memory)
{
    // My platform is little endian
    Word data = memory[address];
    data |= (memory[++address] << 8);
    cycles -= 2;
    return data;
}

/* Read 1 word (2 bytes) from memory AND increment program counter */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::FetchWord(uint32_t& cycles, Memory& memory)
{
    // 6502 is little endian
    Word data = ReadWord(cycles, ProgramCounter, memory);
    ProgramCounter += 2;
    return data;
}

/* Set flags required by a LDA operation */
void CPU::LDA_SetStatus()
{
    F_Zero = (RegA == 0);
    F_Negative = (RegA & 0b10000000) > 0;
}

void CPU::Execute(uint32_t cycles, Memory& memory)
{
    for(cycles; cycles > 0;)
    {
        Instruction instruction = FetchInstruction(cycles, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case INS_LDA_IM:
            byte_Value = FetchByte(cycles, memory);
            RegA = byte_Value;
            LDA_SetStatus();
            break;
        case INS_LDA_ZP:
            byte_Value = FetchByte(cycles, memory);
            RegA = ReadByte(cycles, byte_Value, memory);
            LDA_SetStatus();
            break;
        case INS_LDA_ZPX:
            byte_Value = FetchByte(cycles, memory);
            byte_Value += RegX, cycles--;
            RegA = ReadByte(cycles, byte_Value, memory);
            LDA_SetStatus();
            break;
        case INS_JSR:
            word_Value = FetchWord(cycles, memory);
            memory.WriteWord(StackPointer, ProgramCounter-1, cycles);
            ProgramCounter = word_Value;
            cycles--, StackPointer++;
            break;
        default:
            printf("Unknow instruction \"%#x\" ", instruction);
            break;
        }
    }
}