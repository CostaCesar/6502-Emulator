#include "CPU_Model.h"

class AND_Test : public M6502 {};

TEST_F(AND_Test, AND_Imediatate)
{
    // Given
    processor.RegA = 0b00101010;
    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0b01010101;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegA, 0b01111111);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
}
TEST_F(AND_Test, AND_Imediatate_ZeroFlag)
{
    // Given
    processor.RegA = 0x00;
    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0x00;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegA, 0x00);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
}
TEST_F(AND_Test, AND_Imediatate_NegativeFlag)
{
    // Given
    processor.RegA = 0b01000000;
    memory[0xFFFC] = Instruction::AND_IM;
    memory[0xFFFD] = 0b10000000;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegA, 0b11000000);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
}

// TEST_F(AND_Test, AND_ZeroPage)
// {
//     // Given
//     memory[0xFFFC] = Instruction::AND_ZP;
//     memory[0xFFFD] = 0x10;
//     memory[0x0010] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(3, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 3);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_ZeroPage_OffsetX)
// {
//     // Given
//     processor.RegX = 0x5;
//     memory[0xFFFC] = Instruction::AND_ZPX;
//     memory[0xFFFD] = 0x10;
//     memory[0x0015] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(4, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 4);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_ZeroPage_OffsetX_Wrapping)
// {
//     // Given
//     processor.RegX = 0xFF;
//     memory[0xFFFC] = Instruction::AND_ZPX;
//     memory[0xFFFD] = 0x80;
//     memory[0x007F] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(4, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 4);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Absolute)
// {
//     // Given
//     memory[0xFFFC] = Instruction::AND_AB;
//     memory[0xFFFD] = 0x80;
//     memory[0xFFFE] = 0xA1;
//     memory[0xA180] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(4, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 4);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Absolute_OffsetX)
// {
//     // Given
//     processor.RegX = 0x64;
//     memory[0xFFFC] = Instruction::AND_ABX;
//     memory[0xFFFD] = 0xAA;
//     memory[0xFFFE] = 0xBB;
//     memory[0xBC0E] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(4, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 4);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Absolute_OffsetX_CrossPage)
// {
//     // Given
//     processor.RegX = 0xCC;
//     memory[0xFFFC] = Instruction::AND_ABX;
//     memory[0xFFFD] = 0xAA;
//     memory[0xFFFE] = 0xBB;
//     memory[0xBC76] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(5, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 5);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Absolute_OffsetY)
// {
//     // Given
//     processor.RegY = 0x32;
//     memory[0xFFFC] = Instruction::AND_ABY;
//     memory[0xFFFD] = 0xBB;
//     memory[0xFFFE] = 0xAA;
//     memory[0xAAED] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(4, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 4);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Absolute_OffsetY_CrossPage)
// {
//     // Given
//     processor.RegY = 0xFF;
//     memory[0xFFFC] = Instruction::AND_ABY;
//     memory[0xFFFD] = 0xBB;
//     memory[0xFFFE] = 0xAA;
//     memory[0xABBA] = 0xA;

//     // When
//     uint32_t cycles_executed = processor.Execute(5, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 5);
//     EXPECT_EQ(processor.RegA, 0xA);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Indirect_OffsetX)
// {
//     // Given
//     processor.RegX = 0x2F;
//     memory[0xFFFC] = Instruction::AND_IDX;
//     memory[0xFFFD] = 0x55;
//     memory[0x0084] = 0x22;
//     memory[0x0085] = 0xEF;
//     memory[0xEF22] = 0x26;

//     // When
//     uint32_t cycles_executed = processor.Execute(6, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 6);
//     EXPECT_EQ(processor.RegA, 0x26);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Indirect_OffsetY)
// {
//     // Given
//     processor.RegY = 0xAC;
//     memory[0xFFFC] = Instruction::AND_IDY;
//     memory[0xFFFD] = 0x4C;
//     memory[0x004C] = 0x41;
//     memory[0x004D] = 0x0C;
//     memory[0x0CED] = 0x34;

//     // When
//     uint32_t cycles_executed = processor.Execute(5, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 5);
//     EXPECT_EQ(processor.RegA, 0x34);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }

// TEST_F(AND_Test, AND_Indirect_OffsetY_CrossPage)
// {
//     // Given
//     processor.RegY = 0xE1;
//     memory[0xFFFC] = Instruction::AND_IDY;
//     memory[0xFFFD] = 0x4C;
//     memory[0x004C] = 0x41;
//     memory[0x004D] = 0x0C;
//     memory[0x0D22] = 0x34;

//     // When
//     uint32_t cycles_executed = processor.Execute(6, memory);

//     // Execute
//     EXPECT_EQ(cycles_executed, 6);
//     EXPECT_EQ(processor.RegA, 0x34);
//     EXPECT_FALSE(processor.Flags.Zero);
//     EXPECT_FALSE(processor.Flags.Negative);
//     VerifyUnusedFlags_LD(processor);
// }


