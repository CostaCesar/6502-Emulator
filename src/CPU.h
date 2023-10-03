#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "Base.h"
#include "Memory.h"

struct CPU_Flags
{
    Byte Carry      : 1; 
    Byte Zero       : 1; 
    Byte Interupt   : 1; 
    Byte Decimal    : 1; 
    Byte Break      : 1;
    Byte UNUSED     : 1; 
    Byte OverFlow   : 1; 
    Byte Negative   : 1; 
};
struct CPU
{
    Variant ChipModel = CHIP_STANDART;
    Word ProgramCounter;
    Byte StackPointer;

    Byte RegA, RegX, RegY;

    union // CPU status
    {
        Byte FlagStatus;
        CPU_Flags Flags;
    };
    

    void Reset(Memory& memory);
    void Reset(Word address, Memory& memory);

    void Push_PC_ToStack(uint32_t& cycles, Memory& memory);
    void Pop_PC_FromStack(uint32_t& cycles, Memory& memory);
    Word StackPointer_ToWord() const;
    
    Byte ReadByte(uint32_t& cycles, Word address, const Memory& memory);
    Byte FetchByte(uint32_t& cycles, const Memory& memory);
    Word ReadWord(uint32_t& cycles, Word address, const Memory& memory);
    Word FetchWord(uint32_t& cycles, const Memory& memory);
    Instruction::Opcode FetchInstruction(uint32_t& cycles, const Memory& memory);

    void Check_PageCross(uint32_t& cycles, Word& adrress, Byte offset);
    void IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register);
    void IncrementByRegister(uint32_t& cycles, Word& value, Byte cpu_register);
    void LD_SetStatus(Byte& cpu_register);
    void LD_SetRegister(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory);
    
    uint32_t Execute(uint32_t cycles, Memory& memory);
};

#endif