#include "CPU_Model.h"

class LDA_Test : public M6502 {};

static void VerifyUnusedFlags_LDA(const CPU&processor)
{
    EXPECT_EQ(processor.F_Break, 0);
    EXPECT_EQ(processor.F_Interupt, 0);
    EXPECT_EQ(processor.F_Decimal, 0);
    EXPECT_EQ(processor.F_Carry, 0);
    EXPECT_EQ(processor.F_OverFlow, 0);
}

TEST_F(LDA_Test, LDA_Imediatate)
{
    // Given
    memory[0xFFFC] = INS_LDA_IM;
    memory[0xFFFD] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage)
{
    // Given
    memory[0xFFFC] = INS_LDA_ZP;
    memory[0xFFFD] = 0x10;
    memory[0x0010] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 3);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage_OffsetX)
{
    // Given
    processor.RegX = 0x5;
    memory[0xFFFC] = INS_LDA_ZPX;
    memory[0xFFFD] = 0x10;
    memory[0x0015] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    processor.RegX = 0xFF;
    memory[0xFFFC] = INS_LDA_ZPX;
    memory[0xFFFD] = 0x80;
    memory[0x007F] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Absolute)
{
    // Given
    memory[0xFFFC] = INS_LDA_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;
    memory[0xA180] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetX)
{
    // Given
    processor.RegX = 0x64;
    memory[0xFFFC] = INS_LDA_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC0E] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetX_CrossPage)
{
    // Given
    processor.RegX = 0xCC;
    memory[0xFFFC] = INS_LDA_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;
    memory[0xBC76] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetY)
{
    // Given
    processor.RegY = 0x32;
    memory[0xFFFC] = INS_LDA_ABY;
    memory[0xFFFD] = 0xBB;
    memory[0xFFFE] = 0xAA;
    memory[0xAAED] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Absolute_OffsetY_CrossPage)
{
    // Given
    processor.RegY = 0xFF;
    memory[0xFFFC] = INS_LDA_ABY;
    memory[0xFFFD] = 0xBB;
    memory[0xFFFE] = 0xAA;
    memory[0xABBA] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetX)
{
    // Given
    processor.RegX = 0x2F;
    memory[0xFFFC] = INS_LDA_IDX;
    memory[0xFFFD] = 0x55;
    memory[0x0084] = 0x22;
    memory[0x0085] = 0xEF;
    memory[0xEF22] = 0x26;

    // When
    uint32_t cycles_executed = processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 6);
    EXPECT_EQ(processor.RegA, 0x26);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetY)
{
    // Given
    processor.RegY = 0xAC;
    memory[0xFFFC] = INS_LDA_IDY;
    memory[0xFFFD] = 0x4C;
    memory[0x004C] = 0x41;
    memory[0x004D] = 0x0C;
    memory[0x0CED] = 0x34;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(processor.RegA, 0x34);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}

TEST_F(LDA_Test, LDA_Indirect_OffsetY_CrossPage)
{
    // Given
    processor.RegY = 0xE1;
    memory[0xFFFC] = INS_LDA_IDY;
    memory[0xFFFD] = 0x4C;
    memory[0x004C] = 0x41;
    memory[0x004D] = 0x0C;
    memory[0x0D22] = 0x34;

    // When
    uint32_t cycles_executed = processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 6);
    EXPECT_EQ(processor.RegA, 0x34);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LDA(processor);
}


