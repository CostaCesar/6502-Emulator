#include "../CPU_Model.h"

class STY_Test : public INS_6502 {};

TEST_F(STY_Test, STY_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;

    processor.RegY = VALUE;

    memory[0xFFFC] = Set_6502::STY_ZP;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSIT], VALUE);
    FlagsExcept(processor);
}

TEST_F(STY_Test, STY_ZeroPage_OffsetY)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint32_t POSIT = 0x10;

    processor.RegY = VALUE;
    processor.RegX = OFSET;

    memory[0xFFFC] = Set_6502::STY_ZPX;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSIT + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STY_Test, STY_ZeroPage_OffsetY_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t POSIT = 0xFA;

    processor.RegY = VALUE;
    processor.RegX = OFSET;
    memory[0xFFFC] = Set_6502::STY_ZPX;
    memory[0xFFFD] = POSIT;
    
    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSIT + OFSET)], VALUE);
    FlagsExcept(processor);
}

TEST_F(STY_Test, STY_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;

    processor.RegY = VALUE;

    memory[0xFFFC] = Set_6502::STY_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xA180], VALUE);
    FlagsExcept(processor);
}