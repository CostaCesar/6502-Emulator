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

TEST_F(Stack_Test, TSX_Test_NegativeFlag)
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

TEST_F(Stack_Test, TXS_Test)
{
    // Given
    processor.Reset(memory);
    const uint16_t VALUE = 0xF1;
    processor.RegX = VALUE;
    memory[0xFFFC] = Instruction::TXS;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.StackPointer, VALUE);
    VerifyUnusedFlags_ST(processor);
};

TEST_F(Stack_Test, PHA_Test)
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 3;
    const uint8_t VALUE = 0x64;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::PHA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[processor.Stack_AsWord()+1], processor.RegA);
    EXPECT_FALSE(processor.FlagStatus);
};

TEST_F(Stack_Test, PHP_Test) // Oh no, PHP is invading the system!
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 3;
    const uint8_t VALUE = 0b10101010;
    processor.FlagStatus = VALUE;
    memory[0xFFFC] = Instruction::PHP;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[processor.Stack_AsWord()+1], processor.FlagStatus);
};

TEST_F(Stack_Test, PLA_Test)
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0x64;
    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Instruction::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    VerifyUnusedFlags_LD(processor);
};

TEST_F(Stack_Test, PLA_Test_ZeroFlag)
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0x00;
    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Instruction::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
};

TEST_F(Stack_Test, PLA_Test_NegativeFlag)
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0xF1;
    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Instruction::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    VerifyUnusedFlags_LD(processor);
};


TEST_F(Stack_Test, PLP_Test)
{
    // Given
    processor.Reset(memory);
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0b10101010;
    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Instruction::PLP;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.FlagStatus, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
};




