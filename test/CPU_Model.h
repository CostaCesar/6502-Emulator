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

static void VerifyUnusedFlags_LD(const CPU&processor)
{
    EXPECT_EQ(processor.F_Break, 0);
    EXPECT_EQ(processor.F_Interupt, 0);
    EXPECT_EQ(processor.F_Decimal, 0);
    EXPECT_EQ(processor.F_Carry, 0);
    EXPECT_EQ(processor.F_OverFlow, 0);
}

#endif