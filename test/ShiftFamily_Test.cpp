#include "CPU_Model.h"

class Shift_Test : public M6502 {};

TEST_F(Shift_Test, ASL_RegA_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b00010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_NegvFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b01010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10000000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;
    memory[0xFFFD] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_TRUE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_CarryFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10100000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}