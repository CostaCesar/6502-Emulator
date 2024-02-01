#include "../CPU_Model.h"

class BRK_RTI_Test : public M6502 {};

TEST_F(BRK_RTI_Test, BRK_Test)
{
    // Given
    const uint32_t CYCLES = 7;
    const Word PC_VALUE = 0x8000;
    const Byte PREV_STACK = processor.StackPointer;
    
    memory[0xFFFC] = Instruction::BRK;
    memory.WriteWord(0xFFFE, PC_VALUE);

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, PC_VALUE);
    EXPECT_EQ(processor.StackPointer, PREV_STACK - 3);
    EXPECT_TRUE(processor.Flags.Break);
}

TEST_F(BRK_RTI_Test, BRK_StackValues_Test)
{
    // Given
    const uint32_t CYCLES = 7;
    const Word PC_VALUE = 0x6000;
    const Byte PS_VALUE = 0b11010000;
    const Byte PREV_STACK = processor.StackPointer;
    
    processor.FlagStatus = PS_VALUE;
    memory[0xFFFC] = Instruction::BRK;
    memory.WriteWord(0xFFFE, PC_VALUE);

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, PC_VALUE);
    EXPECT_EQ(processor.StackPointer, PREV_STACK - 3);
    EXPECT_EQ(memory[processor.Stack_AsWord() + 3], 0xFF);
    EXPECT_EQ(memory[processor.Stack_AsWord() + 2], 0xFD);
    EXPECT_EQ(memory[processor.Stack_AsWord() + 1], PS_VALUE);
    EXPECT_TRUE(processor.Flags.Break);
}

TEST_F(BRK_RTI_Test, RTI_Test)
{
    // Given
    const Word CPU_START = 0xFF00;

    processor.Reset(CPU_START, memory);
    const uint32_t CYCLES = 7 + 6;
    const Word PC_VALUE = 0x6000;
    const Byte PS_VALUE = 0b11010000;
    const Byte PREV_STACK = processor.StackPointer;
    
    processor.FlagStatus = PS_VALUE;
    memory[CPU_START] = Instruction::BRK;
    memory.WriteWord(0xFFFE, PC_VALUE);
    memory[PC_VALUE] = Instruction::RTI;
    memory[CPU_START+2] = Instruction::LDA_IM;
    memory[CPU_START+3] = CYCLES;

    // First
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, CPU_START + 2);
    EXPECT_EQ(processor.StackPointer, PREV_STACK);
    EXPECT_EQ(processor.FlagStatus, PS_VALUE);
    
    // Then
    cycles_executed = processor.Execute(2, memory);
    EXPECT_EQ(processor.ProgramCounter, CPU_START + 4);
    EXPECT_EQ(cycles_executed, 2);
    EXPECT_EQ(processor.RegA, CYCLES);
}