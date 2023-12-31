#include "CPU_Model.h"

class EOR_Test : public M6502 {};

TEST_F(EOR_Test, EOR_Imediatate)
{
    // Given
    uint32_t CYCLES = 2;
    processor.RegA = 0b00101010;
    memory[0xFFFC] = Instruction::EOR_IM;
    memory[0xFFFD] = 0b01010101;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01111111);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(EOR_Test, EOR_Imediatate_ZeroFlag)
{
    // Given
    uint32_t CYCLES = 2;
    processor.RegA = 0x00;
    memory[0xFFFC] = Instruction::EOR_IM;
    memory[0xFFFD] = 0x00;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(EOR_Test, EOR_Imediatate_NegativeFlag)
{
    // Given
    uint32_t CYCLES = 2;
    processor.RegA = 0b01000000;
    memory[0xFFFC] = Instruction::EOR_IM;
    memory[0xFFFD] = 0b10000000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_ZeroPage)
{
    // Given
    uint32_t CYCLES = 3;
    processor.RegA = 0b01110000;
    memory[0xFFFC] = Instruction::EOR_ZP;
    memory[0xFFFD] = 0x10;
    memory[0x0010] = 0b00011100;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01101100);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_ZeroPage_OffsetX)
{
    // Given
    uint32_t CYCLES = 4;
    processor.RegX = 0x05;
    processor.RegA = 0b10111100;
    memory[0xFFFC] = Instruction::EOR_ZPX;
    memory[0xFFFD] = 0x10;
    memory[0x0015] = 0b01100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_ZeroPage_OffsetX_Wrapping)
{
    // Given
    uint32_t CYCLES = 4;
    processor.RegX = 0xFF;
    processor.RegA = 0b00111100;
    memory[0xFFFC] = Instruction::EOR_ZPX;
    memory[0xFFFD] = 0x80;
    memory[0x007F] = 0b01100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Absolute)
{
    // Given
    uint32_t CYCLES = 4;
    processor.RegA = 0b01011000;
    memory[0xFFFC] = Instruction::EOR_AB;
    memory[0xFFFD] = 0x64;
    memory[0xFFFE] = 0x32;
    memory[0x3264] = 0b01000010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Absolute_OffsetX)
{
    // Given
    uint32_t CYCLES = 4;
    processor.RegX = 0x64;
    processor.RegA = 0b00111100;
    memory[0xFFFC] = Instruction::EOR_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC0E] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10101110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Absolute_OffsetX_CrossPage)
{
    // Given
    uint32_t CYCLES = 4 + 1;
    processor.RegX = 0xCC;
    processor.RegA = 0b00111100;
    memory[0xFFFC] = Instruction::EOR_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC76] = 0b00010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00101110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Absolute_OffsetY)
{
    /// Given
    uint32_t CYCLES = 4;
    processor.RegY = 0x64;
    processor.RegA = 0b01000010;
    memory[0xFFFC] = Instruction::EOR_ABY;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC0E] = 0b01011000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Absolute_OffsetY_CrossPage)
{
    // Given
    uint32_t CYCLES = 4 + 1;
    processor.RegY = 0xCC;
    processor.RegA = 0b10111100;
    memory[0xFFFC] = Instruction::EOR_ABY;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC76] = 0b00010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10101110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Indirect_OffsetX)
{
    // Given
    uint32_t CYCLES = 6;
    processor.RegA = 0b01000001;
    processor.RegX = 0x10;
    memory[0xFFFC] = Instruction::EOR_IDX;
    memory[0xFFFD] = 0x54;
    memory[0x0064] = 0xAA;
    memory[0x0065] = 0xBB;
    memory[0xBBAA] = 0b01000001;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Indirect_OffsetY)
{
    // Given
    uint32_t CYCLES = 5;
    processor.RegA = 0b01000001;
    processor.RegY = 0xAC;
    memory[0xFFFC] = Instruction::EOR_IDY;
    memory[0xFFFD] = 0x4C;
    memory[0x004C] = 0x41;
    memory[0x004D] = 0x0C;
    memory[0x0CED] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11010011);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(EOR_Test, EOR_Indirect_OffsetY_CrossPage)
{
    // Given
    uint32_t CYCLES = 5 + 1;
    processor.RegA = 0b11111111;
    processor.RegY = 0xE1;
    memory[0xFFFC] = Instruction::EOR_IDY;
    memory[0xFFFD] = 0x4C;
    memory[0x004C] = 0x41;
    memory[0x004D] = 0x0C;
    memory[0x0D22] = 0b01111111;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}


