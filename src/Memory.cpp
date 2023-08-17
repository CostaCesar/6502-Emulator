#include "Memory.h"

void Memory::Initialise()
{
    for(uint32_t i = 0; i < MAX_SIZE; i++)
        Data[i] = 0;
}

/* Write 1 byte from memory */ 
Byte Memory::operator[] (uint32_t address) const
{
    return Data[address];
}

/* Read 1 byte from memory */ 
Byte& Memory::operator[] (uint32_t address)
{
    return Data[address];
}

/* Read 1 word (2 bytes) from memory */
void Memory::WriteWord(uint32_t address, Word value, uint32_t& cycles)
{
    Data[address]   = value & 0xFF;
    Data[address+1] = (value >> 8);
    cycles -= 2;
}