#include "../CPU_Model.h"

class BIT_Test : public INS_6502 {};

TEST_F(BIT_Test, BIT_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x32;
    const Byte VALUE_1 = 0xCC;
    const Byte VALUE_2 = 0xCC;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0x00;
    const Byte VALUE_2 = 0x00;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0b10110010;
    const Byte VALUE_2 = 0b10010110;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0b11110010;
    const Byte VALUE_2 = 0b01010110;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0xCC;
    const Byte VALUE_2 = 0xCC;
    const Word POSITION = 0xCDAB;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_AB;
    memory.WriteWord(0XFFFD, POSITION);
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0b01110111;
    const Byte VALUE_2 = 0b00001000;
    const Word POSITION = 0xCDBA;

    processor.RegA = VALUE_1;
    memory[0xFFFC] = Set_6502::BIT_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0b11111111;
    const Byte VALUE_2 = 0b10000000;
    const Word POSITION = 0x6502;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

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
    const Byte VALUE_1 = 0b01111111;
    const Byte VALUE_2 = 0b01000000;
    const Word POSITION = 0x4004;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::BIT_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

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