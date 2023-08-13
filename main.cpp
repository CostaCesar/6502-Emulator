#include <stdio.h>
#include <stdlib.h>

struct CPU
{
    using Byte = unsigned char;
    using Word = unsigned short;

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

    void Reset()
    {
        ProgramCounter = 0xFFFC;
        StackPointer = 0x0100;
        F_Carry = F_Decimal = F_Zero = F_Interupt = F_Break = F_OverFlow = F_Negative = 0;
        RegA = RegY = RegX = 0;
    }
};

int main(int argc, char ** argv)
{
    CPU processor;
    processor.Reset();
    return 0;
}