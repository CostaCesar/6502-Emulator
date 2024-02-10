#include "../CPU_Model.h"

class ORA_Test : public M6502 {};

TEST_F(ORA_Test, ORA_Imediatate)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0b00101010;

    memory[0xFFFC] = Instruction::ORA_IM;
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
TEST_F(ORA_Test, ORA_Imediatate_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0x00;

    memory[0xFFFC] = Instruction::ORA_IM;
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
TEST_F(ORA_Test, ORA_Imediatate_NegativeFlag)
{
    // Given
    const uint32_t CYCLES = 2;

    processor.RegA = 0b01000000;

    memory[0xFFFC] = Instruction::ORA_IM;
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

TEST_F(ORA_Test, ORA_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x10;

    processor.RegA = 0b01110000;

    memory[0xFFFC] = Instruction::ORA_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = 0b00011100;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01111100);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x05;
    const Byte POSITION = 0x25;

    processor.RegX = OFFSET;
    processor.RegA = 0b10111100;

    memory[0xFFFC] = Instruction::ORA_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + OFFSET] = 0b01100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11111110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0xAB;
    const Byte POSITION = 0xE1;

    processor.RegX = OFFSET;
    processor.RegA = 0b00111100;

    memory[0xFFFC] = Instruction::ORA_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = 0b01100110;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01111110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Word POSITION = 0x8432;

    processor.RegA = 0b01011000;

    memory[0xFFFC] = Instruction::ORA_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = 0b01000010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x64;
    const Word POSITION = 0xAA80;

    processor.RegX = OFFSET;
    processor.RegA = 0b00111100;

    memory[0xFFFC] = Instruction::ORA_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10111110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Absolute_OffsetX_CrossPage)
{
    // Given
    const uint32_t CYCLES = 4 + 1;
    const Byte OFFSET = 0xCC;
    const Word POSITION = 0xBBAA;

    processor.RegX = OFFSET;
    processor.RegA = 0b00111100;

    memory[0xFFFC] = Instruction::ORA_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b00010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00111110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Absolute_OffsetY)
{
    /// Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x64;
    const Word POSITION = 0xAA80;

    processor.RegY = OFFSET;
    processor.RegA = 0b01000010;

    memory[0xFFFC] = Instruction::ORA_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b01011000;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01011010);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Absolute_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 4 + 1;
    const Byte OFFSET = 0xD9;
    const Word POSITION = 0xAA80;

    processor.RegY = OFFSET;
    processor.RegA = 0b10111100;

    memory[0xFFFC] = Instruction::ORA_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b10111110);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Indirect_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte OFFSET = 0xFF;
    const Byte POSITION_1 = 0x32;
    const Word POSITION_2 = 0xAA80;

    processor.RegA = 0b01000001;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ORA_IDX;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Byte) POSITION_1 + OFFSET, POSITION_2);
    memory[POSITION_2] = 0b00000001;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000001);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Indirect_OffsetY)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0x10;
    const Byte POSITION_1 = 0x80;
    const Word POSITION_2 = 0xFA32;

    processor.RegA = 0b01000001;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ORA_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = 0b10010010;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11010011);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(ORA_Test, ORA_Indirect_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5 + 1;
    const Byte OFFSET = 0xFF;
    const Byte POSITION_1 = 0x40;
    const Word POSITION_2 = 0x16EA;

    processor.RegA = 0b00000000;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ORA_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = 0b11111111;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11111111);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}


