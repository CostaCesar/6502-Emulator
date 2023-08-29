#include <gtest/gtest.h>
#include "../src/Memory.h"
#include "../src/CPU.h"

class M6502_Test : public testing::Test
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

TEST_F(M6502_Test, LDA_Imediatate)
{
    // Given
    memory[0xFFFC] = INS_LDA_IM;
    memory[0xFFFD] = 0xA;

    // When
    processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage)
{
    // Given
    memory[0xFFFC] = INS_LDA_ZP;
    memory[0xFFFD] = 0x0010;
    memory[0x0010] = 0xA;

    // When
    processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage_OffsetX)
{
    // Given
    processor.RegX = 0x5;
    memory[0xFFFC] = INS_LDA_ZPX;
    memory[0xFFFD] = 0x0010;
    memory[0x0015] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    processor.RegX = 0xFF;
    memory[0xFFFC] = INS_LDA_ZPX;
    memory[0xFFFD] = 0x0080;
    memory[0x007F] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_Absolute)
{
    // Given
    memory[0xFFFC] = INS_LDA_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;
    memory[0xA180] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_Absolute_OffsetX)
{
    // Given
    processor.RegX = 0x64;
    memory[0xFFFC] = INS_LDA_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC0E] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_Absolute_OffsetY)
{
    // Given
    processor.RegY = 0x32;
    memory[0xFFFC] = INS_LDA_ABY;
    memory[0xFFFD] = 0xBB;
    memory[0xFFFE] = 0xAA;
    memory[0xAAED] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_Indirect_OffsetX)
{
    // Given
    processor.RegX = 0x2F;
    memory[0xFFFC] = INS_LDA_IDX;
    memory[0xFFFD] = 0x0055;
    memory[0x0084] = 0x0022;
    memory[0x0085] = 0x00EF;
    memory[0xEF22] = 0x26;

    // When
    processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0x26);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_Indirect_OffsetY)
{
    // Given
    processor.RegY = 0xAC;
    memory[0xFFFC] = INS_LDA_IDY;
    memory[0xFFFD] = 0x004C;
    memory[0x004C] = 0x0041;
    memory[0x004D] = 0x000C;
    memory[0x0CED] = 0x34;

    // When
    processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0x34);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}


