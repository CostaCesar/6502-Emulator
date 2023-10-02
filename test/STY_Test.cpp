#include "CPU_Model.h"

class STY_Test : public M6502 {};

TEST_F(STY_Test, STY_ZeroPage)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;
    processor.RegY = VALUE;
    memory[0xFFFC] = Instruction::STY_ZP;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 3);
    EXPECT_EQ(memory[POSIT], VALUE);
    VerifyUnusedFlags_ST(processor);
}

TEST_F(STY_Test, STY_ZeroPage_OffsetY)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint32_t POSIT = 0x10;
    processor.RegY = VALUE;
    processor.RegX = OFSET;
    memory[0xFFFC] = Instruction::STY_ZPX;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[POSIT + OFSET], VALUE);
    VerifyUnusedFlags_ST(processor);
}

TEST_F(STY_Test, STY_ZeroPage_OffsetY_Wrapping)
{
    // Given
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t POSIT = 0xFA;
    processor.RegY = VALUE;
    processor.RegX = OFSET;
    memory[0xFFFC] = Instruction::STY_ZPX;
    memory[0xFFFD] = POSIT;
    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[(Byte) (POSIT + OFSET)], VALUE);
    VerifyUnusedFlags_ST(processor);
}

TEST_F(STY_Test, STY_Absolute)
{
    // Given
    const uint16_t VALUE = 0x40;
    processor.RegY = VALUE;
    memory[0xFFFC] = Instruction::STY_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;

    // When
    uint32_t cycles_executed = processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(cycles_executed, 4);
    EXPECT_EQ(memory[0xA180], VALUE);
    VerifyUnusedFlags_ST(processor);
}