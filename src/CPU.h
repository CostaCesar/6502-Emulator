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
    
    CPU(Variant chip_Model, Memory& memory, Word start_from);
    
    uint32_t Execute(uint32_t cycles, Memory& memory);
    void Reset(Memory& memory);
    void Reset(Word address, Memory& memory);
    Word Stack_AsWord() const;  

private:
    void PushByte_Stack(uint32_t& cycles, Byte value, Memory& memory);
    Byte PopByte_Stack(uint32_t& cycles, const Memory& memory);
    void PushWord_Stack(uint32_t& cycles, Word value, Memory& memory);
    Word PopWord_Stack(uint32_t& cycles, const Memory& memory);
    void Push_ProgCount_Stack(uint32_t& cycles, Memory& memory);
    Word Pop_PC_Stack(uint32_t& cycles, Memory& memory);

    Byte ReadByte(uint32_t& cycles, Word address, const Memory& memory);
    Byte FetchByte(uint32_t& cycles, const Memory& memory);
    Word FetchByte_AsWord(uint32_t &cycles, const Memory &memory);
    Word ReadWord(uint32_t& cycles, Word address, const Memory& memory);
    Word FetchWord(uint32_t& cycles, const Memory& memory);
    Instruction::Opcode FetchInstruction(uint32_t& cycles, const Memory& memory);

    void Check_PageCross(uint32_t& cycles, Word& adrress, Byte offset);
    void IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register);
    void IncrementByRegister(uint32_t& cycles, Word& value, Byte cpu_register);
    void Load_Register(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory);
    void Logical_Operate(uint32_t& cycles, Word address, char operation, const Memory& memory);
    void Bit_Test(uint32_t& cycles, Word address, const Memory& memory);
    void Memory_Increment(uint32_t& cycles, Word address, Byte value, Memory& memory);
    void Shift_Value_WithZero(uint32_t& cycles, Byte& value, char direc);
    void Shift_Value_Carrying(uint32_t& cycles, Byte& value, char direc);
    
    void SetStatus_NegvZero(Byte cpu_register);
    void Math_Add(uint32_t& cycles, Byte value);
    void Math_Sub(uint32_t& cycles, Byte value);
    
};

#endif