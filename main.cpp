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
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5;

    /* Functions */
    void Reset(Memory& memory)
    {
        ProgramCounter = 0xFFFC;
        StackPointer = 0x00FF;
        F_Carry = F_Decimal = F_Zero = F_Interupt = F_Break = F_OverFlow = F_Negative = 0;
        RegA = RegY = RegX = 0;
        memory.Initialise();
    }

    Byte ReadByte(uint32_t& cycles, Word address, Memory& memory)
    {
        Byte data = memory[address];
        cycles--;
        return data;
    }
    Byte FetchByte(uint32_t& cycles, Memory& memory)
    {
        Byte data = ReadByte(cycles, ProgramCounter, memory);
        ProgramCounter++;
        return data;
    }
    Word ReadWord(uint32_t& cycles, Word address, Memory& memory)
    {
        // My platform is little endian
        Word data = memory[address];
        data |= (memory[++address] << 8);
        cycles -= 2;
        return data;
    }
    Word FetchWord(uint32_t& cycles, Memory& memory)
    {
        // 6502 is little endian
        Word data = ReadWord(cycles, ProgramCounter, memory);
        ProgramCounter += 2;
        return data;
    }


    void LDA_SetStatus()
    {
        F_Zero = (RegA == 0);
        F_Negative = (RegA & 0b10000000) > 0;
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
                LDA_SetStatus();
                break;
            case INS_LDA_ZP:
                value = FetchByte(cycles, memory);
                RegA = ReadByte(cycles, value, memory);
                LDA_SetStatus();
                break;
            case INS_LDA_ZPX:
                value = FetchByte(cycles, memory);
                value += RegX, cycles--;
                RegA = ReadByte(cycles, value, memory);
                LDA_SetStatus();
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
    processor.RegX = 0x08;
    memory[0xFFFC] = CPU::INS_LDA_ZPX;
    memory[0xFFFD] = 0x0F;
    memory[0x000F + 0x08] = 0xFF;
    // end - Hacked code

    processor.Execute(4, memory);
    return 0;
}