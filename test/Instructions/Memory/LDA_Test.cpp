#include "../CPU_Model.h"

class LDA_Test : public M6502 {};

TEST_F(LDA_Test, LDA_Imediatate)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0xA;

    memory[0xFFFC] = Instruction::LDA_IM;
    memory[0xFFFD] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x10;

    memory[0xFFFC] = Instruction::LDA_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0x21;
    const Byte POSITION = 0x20;
    const Byte OFFSET = 0x5;

    processor.RegX = 0x5;

    memory[0xFFFC] = Instruction::LDA_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0x32;
    const Byte POSITION = 0x40;
    const Byte OFFSET = 0xF1;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDA_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0x32;
    const Word POSITION = 0x200F;

    memory[0xFFFC] = Instruction::LDA_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x64;
    const Word POSITION = 0xAA80;
    const Byte VALUE = 0x57;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDA_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetX_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0xDE;
    const Word POSITION = 0xAABB;
    const Byte VALUE = 0x57;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDA_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetY)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x16;
    const Word POSITION = 0x1940;
    const Byte VALUE = 0x66;

    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::LDA_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0xEF;
    const Word POSITION = 0xAE86;
    const Byte VALUE = 0xAB;

    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::LDA_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte OFFSET = 0x72;
    const Byte POSITION_1 = 0x03;
    const Word POSITION_2 = 0xF005;
    const Byte VALUE = 0x01;

    processor.RegX = OFFSET;
    
    memory[0xFFFC] = Instruction::LDA_IDX;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Byte) POSITION_1 + OFFSET, POSITION_2);
    memory[POSITION_2] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetY)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0x12;
    const Byte POSITION_1 = 0x90;
    const Word POSITION_2 = 0x2077;
    const Byte VALUE = 0x40;

    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::LDA_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte OFFSET = 0xFA;
    const Byte POSITION_1 = 0x55;
    const Word POSITION_2 = 0xF0A5;
    const Byte VALUE = 0xFF;
    
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::LDA_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}


