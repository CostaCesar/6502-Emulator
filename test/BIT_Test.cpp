#include "CPU_Model.h"

class BIT_Test : public M6502 {};

TEST_F(BIT_Test, BIT_ZeroPage)
{
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x32;
    processor.RegA = 0xCC;
    memory[0xFFFC] = Instruction::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = 0xCC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0xCC);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_ZeroPage_ZeroFlag)
{
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x22;
    processor.RegA = 0x00;
    memory[0xFFFC] = Instruction::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = 0x00;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_ZeroPage_NegativeFlag)
{
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x39;
    processor.RegA = 0b10110010;
    memory[0xFFFC] = Instruction::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = 0b10010110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10110010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_ZeroPage_OverflowFlag)
{
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x30;
    processor.RegA = 0b11110010;
    memory[0xFFFC] = Instruction::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = 0b01010110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11110010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_Absolute)
{
    const uint32_t CYCLES = 4;
    processor.RegA = 0xCC;
    memory[0xFFFC] = Instruction::BIT_AB;
    memory[0xFFFD] = 0xAB;
    memory[0xFFFE] = 0xCD;
    memory[0xCDAB] = 0xCC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0xCC);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_Absolute_ZeroFlag)
{
    const uint32_t CYCLES = 4;
    processor.RegA = 0b01110111;
    memory[0xFFFC] = Instruction::BIT_AB;
    memory[0xFFFD] = 0xAB;
    memory[0xFFFE] = 0xCD;
    memory[0xCDAB] = 0b00001000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01110111);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_Absolute_NegativeFlag)
{
    const uint32_t CYCLES = 4;
    processor.RegA = 0b11111111;
    memory[0xFFFC] = Instruction::BIT_AB;
    memory[0xFFFD] = 0xAB;
    memory[0xFFFE] = 0xCD;
    memory[0xCDAB] = 0b10000000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11111111);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(BIT_Test, BIT_Absolute_OverflowFlag)
{
    const uint32_t CYCLES = 4;
    processor.RegA = 0b01111111;
    memory[0xFFFC] = Instruction::BIT_AB;
    memory[0xFFFD] = 0xAB;
    memory[0xFFFE] = 0xCD;
    memory[0xCDAB] = 0b01000000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01111111);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}