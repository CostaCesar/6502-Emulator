#include "CPU_Model.h"

class LDY_Test : public M6502 {};

TEST_F(LDY_Test, LDY_Imediatate)
{
    // Given
    memory[0xFFFC] = INS_LDY_IM;
    memory[0xFFFD] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage)
{
    // Given
    memory[0xFFFC] = INS_LDY_ZP;
    memory[0xFFFD] = 0x10;
    memory[0x0010] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 3);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage_OffsetY)
{
    // Given
    processor.RegX = 0x5;
    memory[0xFFFC] = INS_LDY_ZPY;
    memory[0xFFFD] = 0x10;
    memory[0x0015] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage_OffsetY_Wrapping)
{
    // Given
    processor.RegX = 0xFF;
    memory[0xFFFC] = INS_LDY_ZPY;
    memory[0xFFFD] = 0x80;
    memory[0x007F] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_Absolute)
{
    // Given
    memory[0xFFFC] = INS_LDY_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;
    memory[0xA180] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_Absolute_OffsetY)
{
    // Given
    processor.RegX = 0x32;
    memory[0xFFFC] = INS_LDY_ABY;
    memory[0xFFFD] = 0xBB;
    memory[0xFFFE] = 0xAA;
    memory[0xAAED] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}

TEST_F(LDY_Test, LDY_Absolute_OffsetY_CrossPage)
{
    // Given
    processor.RegX = 0xFF;
    memory[0xFFFC] = INS_LDY_ABY;
    memory[0xFFFD] = 0xBB;
    memory[0xFFFE] = 0xAA;
    memory[0xABBA] = 0xA;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(processor.RegY, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
    VerifyUnusedFlags_LD(processor);
}