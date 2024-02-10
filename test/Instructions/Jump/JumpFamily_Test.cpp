#include "../CPU_Model.h"

class Jump_Test : public M6502 {};

TEST_F(Jump_Test, JSR)
{
    // Given
    const uint32_t CYCLES = 8;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;
    memory[POSIT] = Instruction::LDA_IM;
    memory[POSIT+1] = VALUE;
    memory[0xFFFC] = Instruction::JSR;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF - 2);
    EXPECT_EQ(processor.ProgramCounter, POSIT + 2);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, RTS)
{
    // Given
    const uint32_t CYCLES = 6 + 6 + 2;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;

    processor.Reset(0xFF00);
    memory[0xFF00] = Instruction::JSR;
    memory.WriteWord(0xFF01, POSIT);
    memory[POSIT] = Instruction::RTS;
    memory[0xFF03] = Instruction::LDA_IM;
    memory[0xFF04] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_AB)
{
    // Given
    const uint32_t CYCLES = 5;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x1010;

    processor.Reset(0xFF00);
    memory[0xFF00] = Instruction::JMP_AB;
    memory.WriteWord(0xFF01, POSIT);
    memory[POSIT] = Instruction::LDA_IM;
    memory[POSIT+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID)
{
    // Given
    const uint32_t CYCLES = 7;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x1010;
    const uint32_t POSIT_2 = 0xFF20;
    
    processor.Reset(0xFF00);
    processor.ChipModel = CHIP_STANDART;
    
    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory.WriteWord(POSIT_1, POSIT_2);
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID_StandartChip_PageCross)
{
    // Given
    const uint32_t CYCLES = 7;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x10FF;
    const uint32_t POSIT_2 = 0xFF20;

    processor.Reset(0xFF00);
    processor.ChipModel = CHIP_STANDART;

    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory[POSIT_1] = 0x0020;
    memory[0x1000] = 0x00FF;
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

TEST_F(Jump_Test, JMP_ID_65SC02Chip_PageCross)
{
    // Given
    const uint32_t CYCLES = 7;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT_1 = 0x1010;
    const uint32_t POSIT_2 = 0xFF20;

    processor.Reset(0xFF00);
    processor.ChipModel = CHIP_65SC02;

    memory[0xFF00] = Instruction::JMP_ID;
    memory.WriteWord(0xFF01, POSIT_1);
    memory.WriteWord(POSIT_1, POSIT_2);
    memory[POSIT_2] = Instruction::LDA_IM;
    memory[POSIT_2+1] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.ProgramCounter, POSIT_2+2);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_EQ(processor.FlagStatus, 0x0000);
}

