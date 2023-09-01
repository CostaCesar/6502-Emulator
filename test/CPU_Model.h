#ifndef CPU_MODEL_H
#define CPU_MODEL_H

#include <gtest/gtest.h>
#include "../src/Memory.h"
#include "../src/CPU.h"

class M6502 : public testing::Test
{
public:
    Memory memory;
    CPU processor;

    virtual void SetUp()
    {
        processor.Reset(memory);
    }

    virtual void TearDown()
    {

    }
};

#endif