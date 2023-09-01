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
    cycles++;
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
    cycles += 2;
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
            byte_Value = FetchByte(cycles_ran, memory);
            RegA = byte_Value;
            LDA_SetStatus();
            break;
        case INS_LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA = ReadByte(cycles_ran, byte_Value, memory);
            LDA_SetStatus();
            break;
        case INS_LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value += RegX, cycles_ran++;
            RegA = ReadByte(cycles_ran, byte_Value, memory);
            LDA_SetStatus();
            break;
        case INS_LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            RegA = ReadByte(cycles_ran, word_Value, memory);
            LDA_SetStatus();
            break;
        case INS_LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            if((int) (word_Value / 0xFF) < (int) ((word_Value + RegX) / 0xFF))
            { /* Should do something with the cycles*/ cycles_ran++; }
            word_Value += RegX;
            RegA = ReadByte(cycles_ran, word_Value, memory);
            break;
        case INS_LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            if((int) (word_Value / 0xFF) < (int) ((word_Value + RegY) / 0xFF))
            { /* Should do something with the cycles*/ cycles_ran++; }
            word_Value += RegY;
            RegA = ReadByte(cycles_ran, word_Value, memory);
            break;
        case INS_LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value += RegX, cycles_ran++;
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            RegA = ReadByte(cycles_ran, word_Value, memory);
            break;
        case INS_LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            if((int) (word_Value / 0xFF) < (int) ((word_Value + RegY) / 0xFF))
            { /* Should do something with the cycles*/ cycles_ran++; }
            word_Value += RegY;
            RegA = ReadByte(cycles_ran, word_Value, memory);
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