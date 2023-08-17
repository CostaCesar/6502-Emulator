#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "Base.h"

struct Memory
{
    static constexpr uint32_t MAX_SIZE = 1024 * 64;
    Byte Data[MAX_SIZE];

    void Initialise();

    Byte operator[] (uint32_t address) const; /* Write 1 byte to memory */ 
    Byte& operator[] (uint32_t address); /* Read 1 byte from memory */ 
    void WriteWord(uint32_t address, Word value, uint32_t& cycles); /* Read 1 word (2 bytes) from memory */
};

#endif