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

    /* Read 1 word (2 bytes) from memory */
    void WriteWord(uint32_t address, Word value, uint32_t& cycles)
    {
        Data[address]   = value & 0xFF;
        Data[address+1] = (value >> 8);
        cycles -= 2;
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
        INS_LDA_IM = 0xA9,  // 2 cycles: Load To RegA Imediate value
        INS_LDA_ZP = 0xA5,  // 3 cyles: Load to RegA value from memory
        INS_LDA_ZPX = 0xB5, // 4 cyles: Load to RegA value from memory + offset from RegX
        INS_JSR = 0x20;     // 6 cyles: Branch to subroutine

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
};

int main(int argc, char ** argv)
{
    Memory memory;
    CPU processor;
    processor.Reset(memory);
    
    // start - Hacked code
    memory[0xFFFC] = CPU::INS_JSR;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xAA;
    memory[0xAAAA] = CPU::INS_LDA_IM;
    memory[0xAAAB] = 0xA;
    // end - Hacked code

    processor.Execute(8, memory);
    return 0;
}