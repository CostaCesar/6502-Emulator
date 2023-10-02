#include "CPU_Model.h"

class JSR_Test : public M6502 {};

TEST_F(JSR_Test, JSR_Basic)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;
    memory[POSIT] = Instruction::LDA_IM;
    memory[POSIT+1] = VALUE;
    memory[0xFFFC] = Instruction::JSR;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(8, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 8);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF - 2);
    EXPECT_EQ(processor.ProgramCounter, POSIT + 2);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(JSR_Test, RTS_Basic)
{
    // Given
    processor.Reset(0xFF00, memory);
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;
    memory[0xFF00] = Instruction::JSR;
    memory[0xFF01] = POSIT;
    memory[POSIT] = Instruction::RTS;
    memory[0xFF03] = Instruction::LDA_IM;
    memory[0xFF04] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(14, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 14);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}
