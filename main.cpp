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
};

int main(int argc, char ** argv)
{
    CPU processor;
    return 0;
}