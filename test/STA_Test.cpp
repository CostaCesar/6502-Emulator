#include "CPU_Model.h"

class STA_Test : public M6502 {};

TEST_F(STA_Test, STA_ZeroPage)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_ZP;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 3);
    EXPECT_EQ(memory[POSIT], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_ZeroPage_OffsetX)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint32_t POSIT = 0x10;
    processor.RegA = VALUE;
    processor.RegX = OFSET;
    memory[0xFFFC] = Instruction::STA_ZPX;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[POSIT + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t POSIT = 0xFA;
    processor.RegA = VALUE;
    processor.RegX = OFSET;
    memory[0xFFFC] = Instruction::STA_ZPX;
    memory[0xFFFD] = POSIT;
    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[(Byte) (POSIT + OFSET)], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute)
{
    // Given
    const uint16_t VALUE = 0x40;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[0xA180], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute_OffsetX)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    processor.RegX = OFSET;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(memory[0xBBAA + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute_OffsetY)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    processor.RegY = OFSET;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_ABY;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;

    // When
    uint32_t cycles_executed = processor.Execute(5, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 5);
    EXPECT_EQ(memory[0xBBAA + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Indirect_OffsetX)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t ZPPOS = 0x20;
    processor.RegX = OFSET;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_IDX;
    memory[0xFFFD] = ZPPOS - OFSET;
    memory[ZPPOS]  = 0xBA;
    memory[ZPPOS+1]= 0xDC;

    // When
    uint32_t cycles_executed = processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 6);
    EXPECT_EQ(memory[0xDCBA], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Indirect_OffsetY)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t ZPPOS = 0x20;
    processor.RegY = OFSET;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::STA_IDY;
    memory[0xFFFD] = ZPPOS;
    memory[ZPPOS]  = 0xBA;
    memory[ZPPOS+1]= 0xDC;

    // When
    uint32_t cycles_executed = processor.Execute(6, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 6);
    EXPECT_EQ(memory[0xDCBA+OFSET], VALUE);
    FlagsExcept(processor);
}

