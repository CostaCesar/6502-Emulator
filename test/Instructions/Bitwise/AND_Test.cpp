#include "../CPU_Model.h"

class AND_Test : public M6502 {};

TEST_F(AND_Test, AND_Imediatate)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0b01010101;

    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0b01000001;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000001);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(AND_Test, AND_Imediatate_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0b10101010;

    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0b01010101;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(AND_Test, AND_Imediatate_NegativeFlag)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0b11000000;

    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0b10000001;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;

    processor.RegA = 0b01110000;

    memory[0xFFFC] = Instruction::AND_ZP;
    memory[0xFFFD] = 0x10;
    memory[0x0010] = 0b00011100;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00010000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;

    processor.RegX = 0x05;
    processor.RegA = 0b10111100;

    memory[0xFFFC] = Instruction::AND_ZPX;
    memory[0xFFFD] = 0x10;
    memory[0x0015] = 0b11100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10100100);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;

    processor.RegX = 0xFF;
    processor.RegA = 0b00111100;

    memory[0xFFFC] = Instruction::AND_ZPX;
    memory[0xFFFD] = 0x80;
    memory[0x007F] = 0b01100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00100100);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Word POSITION = 0x3264;

    processor.RegA = 0b01011000;

    memory[0xFFFC] = Instruction::AND_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = 0b01000010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x80;
    const Word POSITION = 0xBB00;

    processor.RegX = OFFSET;
    processor.RegA = 0b10111100;

    memory[0xFFFC] = Instruction::AND_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10010000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Absolute_OffsetX_CrossPage)
{
    // Given
    const uint32_t CYCLES = 4 + 1;
    const Byte OFFSET = 0xCC;
    const Word POSITION = 0xBBAA;

    processor.RegX = OFFSET;
    processor.RegA = 0b00111100;

    memory[0xFFFC] = Instruction::AND_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b00010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00010000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Absolute_OffsetY)
{
    /// Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x16;
    const Word POSITION = 0xCF06;

    processor.RegY = OFFSET;
    processor.RegA = 0b01000010;

    memory[0xFFFC] = Instruction::AND_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b01011000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Absolute_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 4 + 1;
    const Byte OFFSET = 0xCC;
    const Word POSITION = 0xBBAA;

    processor.RegY = OFFSET;
    processor.RegA = 0b10111100;

    memory[0xFFFC] = Instruction::AND_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10010000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Indirect_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte OFFSET = 0x02;
    const Byte POSITION_1 = 0x0E;
    const Word POSITION_2 = 0xBBAA;

    processor.RegA = 0b01000001;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::AND_IDX;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1 + OFFSET, POSITION_2);
    memory[POSITION_2] = 0b01000001;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000001);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Indirect_OffsetY)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0xAC;
    const Byte POSITION_1 = 0x4C;
    const Word POSITION_2 = 0x0C41;

    processor.RegA = 0b01000001;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::AND_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(AND_Test, AND_Indirect_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5 + 1;
    const Byte OFFSET = 0xE1;
    const Byte POSITION_1 = 0x4C;
    const Word POSITION_2 = 0xBBAA;

    processor.RegA = 0b00000000;
    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::AND_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = 0b11111111;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}


