#include "../CPU_Model.h"

class STA_Test : public INS_6502 {};

TEST_F(STA_Test, STA_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const uint16_t VALUE = 0x40;
    const uint32_t POSIT = 0x10;

    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_ZP;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSIT], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint32_t POSIT = 0x10;

    processor.RegA = VALUE;
    processor.RegX = OFSET;

    memory[0xFFFC] = Set_6502::STA_ZPX;
    memory[0xFFFD] = POSIT;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSIT + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t POSIT = 0xFA;

    processor.RegA = VALUE;
    processor.RegX = OFSET;

    memory[0xFFFC] = Set_6502::STA_ZPX;
    memory[0xFFFD] = POSIT;
    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSIT + OFSET)], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint16_t VALUE = 0x40;

    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_AB;
    memory[0xFFFD] = 0x80;
    memory[0xFFFE] = 0xA1;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xA180], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 5;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;

    processor.RegX = OFSET;
    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_ABX;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xBBAA + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Absolute_OffsetY)
{
    // Given
    const uint32_t CYCLES = 5;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;

    processor.RegY = OFSET;
    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_ABY;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xBB;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xBBAA + OFSET], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Indirect_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t Z_POS = 0x20;

    processor.RegX = OFSET;
    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_IDX;
    memory[0xFFFD] = Z_POS - OFSET;
    memory[Z_POS]  = 0xBA;
    memory[Z_POS+1]= 0xDC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xDCBA], VALUE);
    FlagsExcept(processor);
}

TEST_F(STA_Test, STA_Indirect_OffsetY)
{
    // Given
    const uint32_t CYCLES = 6;
    const uint16_t VALUE = 0x40;
    const uint16_t OFSET = 0xA;
    const uint16_t Z_POS = 0x20;

    processor.RegY = OFSET;
    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::STA_IDY;
    memory[0xFFFD] = Z_POS;
    memory[Z_POS]  = 0xBA;
    memory[Z_POS+1]= 0xDC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[0xDCBA+OFSET], VALUE);
    FlagsExcept(processor);
}

