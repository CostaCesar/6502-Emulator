#include "CPU_Model.h"

class Stack_Test : public M6502 {};

TEST_F(Stack_Test, TSX_Test_Basic)
{
    // Given
    processor.Reset(memory);
    const uint16_t VALUE = 0x01;
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = true;
    memory[0xFFFC] = Instruction::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
};

TEST_F(Stack_Test, TSX_Test_ZeroFlag)
{
    // Given
    processor.Reset(memory);
    const uint16_t VALUE = 0x00;
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = false;
    memory[0xFFFC] = Instruction::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
};

TEST_F(Stack_Test, TSX_Test_Presist)
{
    // Given
    processor.Reset(memory);
    const uint16_t VALUE = 0xF1;
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = false;
    memory[0xFFFC] = Instruction::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
};