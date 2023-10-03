#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "Base.h"

struct Memory
{
    static constexpr uint32_t MAX_SIZE = 1024 * 64;
    Byte Data[MAX_SIZE];

    void Initialise();

    Byte& operator[] (uint32_t address);  /* Write to memory */ 
    Byte operator[] (uint32_t address) const; /* Read from memory */ 
    void WriteWord(uint32_t address, const Word value, uint32_t& cycles); /* Write 1 word (2 bytes) to memory */
    void WriteWord(uint32_t address, const Word value); /* Write 1 word (2 bytes) to memory, without cycle*/
    void WriteByte(uint32_t address, const Byte value, uint32_t& cycles); /* Write 1 byte to memory */
};

#endif