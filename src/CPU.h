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

    void Reset(Memory& memory);
    
    Byte ReadByte(uint32_t& cycles, Word address, const Memory& memory);
    Byte FetchByte(uint32_t& cycles, const Memory& memory);
    Word ReadWord(uint32_t& cycles, Word address, const Memory& memory);
    Word FetchWord(uint32_t& cycles, const Memory& memory);
    Instruction FetchInstruction(uint32_t& cycles, const Memory& memory);

    void Check_PageCross(uint32_t& cycles, Word& adrress, Byte offset);
    void IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register);
    void LD_SetStatus(Byte& cpu_register);
    void LD_SetRegister(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory);
    
    uint32_t Execute(uint32_t cycles, Memory& memory);
};

#endif