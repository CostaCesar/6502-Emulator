#include "../CPU_Model.h"

class CPY_Test : public M6502 {};

TEST_F(CPY_Test, CPY_Immediate)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE_1 = 0xBB;
    const Byte VALUE_2 = 0xAA;

    processor.RegY = VALUE_1;

    memory[0xFFFC] = Instruction::CPY_IM;
    memory[0xFFFD] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE_1);
    EXPECT_EQ(memory[0xFFFD], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CPY_Test, CPY_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte VALUE_1 = 0xCC;
    const Byte VALUE_2 = 0xCC;
    const Byte POSITION = 0x10;

    processor.RegY = VALUE_1;

    memory[0xFFFC] = Instruction::CPY_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE_1);
    EXPECT_EQ(memory[POSITION], VALUE_2);

    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CPY_Test, CPY_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0xAA;
    const Byte VALUE_2 = 0xFF;
    const Word POSITION = 0x9876;

    processor.RegY = VALUE_1;

    memory[0xFFFC] = Instruction::CPY_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE_1);
    EXPECT_EQ(memory[POSITION], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}