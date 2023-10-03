#include "CPU_Model.h"

class Jump_Test : public M6502 {};

TEST_F(Jump_Test, JSR_Test)
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

TEST_F(Jump_Test, RTS_Test)
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

TEST_F(Jump_Test, JMP_AB_Test)
{
    // Given
    processor.Reset(0xFF00, memory);
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x1010;
    memory[0xFF00] = Instruction::JMP_AB;
    memory.WriteWord(0xFF01, POSIT);
    memory[POSIT] = Instruction::LDA_IM;
    memory[POSIT+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID_Test)
{
    // Given
    processor.Reset(0xFF00, memory);
    processor.ChipModel = CHIP_STANDART;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x1010;
    const uint32_t POSIT_2 = 0xFF20;
    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory.WriteWord(POSIT_1, POSIT_2);
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(7, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 7);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID_StandartChip_PageCross)
{
    // Given
    processor.Reset(0xFF00, memory);
    processor.ChipModel = CHIP_STANDART;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x10FF;
    const uint32_t POSIT_2 = 0xFF20;
    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory[POSIT_1] = 0x0020;
    memory[0x1000] = 0x00FF;
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(7, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 7);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID_65SC02Chip_PageCross)
{
    // Given
    processor.Reset(0xFF00, memory);
    processor.ChipModel = CHIP_65SC02;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x1010;
    const uint32_t POSIT_2 = 0xFF20;
    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory.WriteWord(POSIT_1, POSIT_2);
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(7, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 7);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

