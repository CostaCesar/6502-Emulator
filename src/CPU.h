#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "Base.h"
#include "Memory.h"

struct CPU
{
    Word ProgramCounter;
    Byte StackPointer;

    Byte RegA, RegX, RegY;

    Byte F_Carry : 1; 
    Byte F_Zero : 1; 
    Byte F_Interupt : 1; 
    Byte F_Decimal : 1; 
    Byte F_Break : 1; 
    Byte F_OverFlow : 1; 
    Byte F_Negative : 1; 

    /* Opcodes */
    static constexpr Byte
        INS_LDA_IM = 0xA9,  // 2 cycles: Load To RegA Imediate value
        INS_LDA_ZP = 0xA5,  // 3 cyles: Load to RegA value from memory
        INS_LDA_ZPX = 0xB5, // 4 cyles: Load to RegA value from memory + offset from RegX
        INS_JSR = 0x20;     // 6 cyles: Branch to subroutine

    void Reset(Memory& memory);
    Byte ReadByte(uint32_t& cycles, Word address, Memory& memory);
    Byte FetchByte(uint32_t& cycles, Memory& memory);
    Word ReadWord(uint32_t& cycles, Word address, Memory& memory);
    Word FetchWord(uint32_t& cycles, Memory& memory);
    void LDA_SetStatus();
    void Execute(uint32_t cycles, Memory& memory);
};

#endif