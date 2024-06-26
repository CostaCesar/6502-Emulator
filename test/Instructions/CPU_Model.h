#ifndef CPU_INS_MODEL_H
#define CPU_INS_MODEL_H

#include <gtest/gtest.h>
#include "../../src/Memory.h"
#include "../../src/CPU.h"

class INS_6502 : public testing::Test
{
public:
    Memory memory;
    CPU processor = CPU(CHIP_STANDART, 0xFFFC);

    virtual void SetUp()
    {
        memory.Initialise();
    }

    virtual void TearDown()
    {

    }
};

static void FlagsExcept_NegvZero(const CPU&processor)
{
    EXPECT_EQ(processor.Flags.Break, 0);
    EXPECT_EQ(processor.Flags.Interupt, 0);
    EXPECT_EQ(processor.Flags.Decimal, 0);
    EXPECT_EQ(processor.Flags.Carry, 0);
    EXPECT_EQ(processor.Flags.OverFlow, 0);
}
static void FlagsExcept(const CPU&processor)
{
    EXPECT_EQ(processor.Flags.Break, 0);
    EXPECT_EQ(processor.Flags.Zero, 0);
    EXPECT_EQ(processor.Flags.Negative, 0);
    EXPECT_EQ(processor.Flags.Interupt, 0);
    EXPECT_EQ(processor.Flags.Decimal, 0);
    EXPECT_EQ(processor.Flags.Carry, 0);
    EXPECT_EQ(processor.Flags.OverFlow, 0);
}
static void FlagsExcept_NegvZeroCarry(const CPU&processor)
{
    EXPECT_EQ(processor.Flags.Break, 0);
    EXPECT_EQ(processor.Flags.Interupt, 0);
    EXPECT_EQ(processor.Flags.Decimal, 0);
    EXPECT_EQ(processor.Flags.OverFlow, 0);
}
#endif