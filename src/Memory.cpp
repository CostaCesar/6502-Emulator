#include "Memory.h"

void Memory::Initialise()
{
    for(uint32_t i = 0; i < MAX_SIZE; i++)
        Data[i] = 0;
}

/* Read from memory */ 
Byte Memory::operator[] (uint32_t address) const
{
    return Data[address];
}

/* Write to memory */ 
Byte& Memory::operator[] (uint32_t address)
{
    return Data[address];
}

/* Write 1 word (2 bytes) to memory */
void Memory::WriteWord(uint32_t address, const Word value, uint32_t& cycles)
{
    WriteWord(address, value);
    cycles += 2;
}
/* Write 1 word (2 bytes) to memory, without cycle*/
void Memory::WriteWord(uint32_t address, const Word value)
{
    Data[address]   = value & 0xFF;
    Data[address+1] = (value >> 8);
}
/* Write 1 byte to memory */
void Memory::WriteByte(uint32_t address, const Byte value, uint32_t& cycles)
{
    Data[address] = value;
    cycles++;
}