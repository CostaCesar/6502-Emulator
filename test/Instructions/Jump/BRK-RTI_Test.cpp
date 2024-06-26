#include "../CPU_Model.h"

class BRK_RTI_Test : public INS_6502 {};

TEST_F(BRK_RTI_Test, BRK)
{
    // Given
    const uint32_t CYCLES = 7;
    const Word PC_VALUE = 0x8000;
    const Byte PREV_STACK = processor.StackPointer;
    
    memory[0xFFFC] = Set_6502::BRK;
    memory.WriteWord(0xFFFE, PC_VALUE);

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, PC_VALUE);
    EXPECT_EQ(processor.StackPointer, PREV_STACK - 3);
    EXPECT_TRUE(processor.Flags.Break);
}

TEST_F(BRK_RTI_Test, BRK_StackValues)
{
    // Given
    const uint32_t CYCLES = 7;
    const Word PC_VALUE = 0x6000;
    const Byte PS_VALUE = 0b11010000;
    const Byte PREV_STACK = processor.StackPointer;
    
    processor.FlagStatus = PS_VALUE;
    memory[0xFFFC] = Set_6502::BRK;
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

TEST_F(BRK_RTI_Test, RTI)
{
    // Given
    const Word CPU_START = 0xFF00;

    const uint32_t CYCLES_1 = 7 + 6;
    const uint32_t CYCLES_2 = 2;
    const Word PC_VALUE = 0x6000;
    const Byte PS_VALUE = 0b11010000;
    const Byte PREV_STACK = processor.StackPointer;
    
    processor.Reset(CPU_START);
    processor.FlagStatus = PS_VALUE;

    memory[CPU_START] = Set_6502::BRK;
    memory.WriteWord(0xFFFE, PC_VALUE);
    memory[PC_VALUE] = Set_6502::RTI;
    memory[CPU_START+2] = Set_6502::LDA_IM;
    memory[CPU_START+3] = PS_VALUE;

    // First
    uint32_t cycles_executed = processor.Execute(CYCLES_1, memory);
    EXPECT_EQ(cycles_executed, CYCLES_1);
    EXPECT_EQ(processor.ProgramCounter, CPU_START + 2);
    EXPECT_EQ(processor.StackPointer, PREV_STACK);
    EXPECT_EQ(processor.FlagStatus, PS_VALUE);
    
    // Then
    cycles_executed += processor.Execute(2, memory);
    EXPECT_EQ(processor.ProgramCounter, CPU_START + 4);
    EXPECT_EQ(cycles_executed, CYCLES_1 + CYCLES_2);
    EXPECT_EQ(processor.RegA, PS_VALUE);
}