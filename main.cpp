#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

using Byte = unsigned char;
using Word = unsigned short;

struct Memory
{
    static constexpr uint32_t MAX_SIZE = 1024 * 64;
    Byte Data[MAX_SIZE];

    void Initialise()
    {
        for(uint32_t i = 0; i < MAX_SIZE; i++)
            Data[i] = 0;
    }

    /* Read 1 byte from memory */ 
    Byte operator[] (uint32_t address) const
    {
        return Data[address];
    }

    /* Read 1 byte from memory */ 
    Byte& operator[] (uint32_t address)
    {
        return Data[address];
    }
};

struct CPU
{

    Word ProgramCounter;
    Word StackPointer;

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
        INS_LDA_IM = 0xA9;

    /* Functions */
    void Reset(Memory& memory)
    {
        ProgramCounter = 0xFFFC;
        StackPointer = 0x0100;
        F_Carry = F_Decimal = F_Zero = F_Interupt = F_Break = F_OverFlow = F_Negative = 0;
        RegA = RegY = RegX = 0;
        memory.Initialise();
    }

    Byte FetchByte(uint32_t& cycles, Memory& memory)
    {
        Byte data = memory[ProgramCounter];
        ProgramCounter++, cycles--;
        return data;
    }

    void Execute(uint32_t cycles, Memory& memory)
    {
        for(cycles; cycles > 0;)
        {
            Byte instruction = FetchByte(cycles, memory);
            Byte value = 0;
            switch (instruction)
            {
            case INS_LDA_IM:
                value = FetchByte(cycles, memory);
                RegA = value;
                F_Zero = (RegA == 0);
                F_Negative = (RegA & 0b10000000) > 0;
                break;
            default:
                printf("Unknow instruction \"%#x\" ", instruction);
                break;
            }
        }
    }
};

int main(int argc, char ** argv)
{
    Memory memory;
    CPU processor;
    processor.Reset(memory);
    
    // start - Hacked code
    int cycles = 2;
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0xF;
    // end - Hacked code

    processor.Execute(cycles, memory);
    return 0;
}