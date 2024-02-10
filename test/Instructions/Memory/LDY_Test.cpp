#include "../CPU_Model.h"

class LDY_Test : public M6502 {};

TEST_F(LDY_Test, LDY_Imediatate)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0xA;

    memory[0xFFFC] = Instruction::LDY_IM;
    memory[0xFFFD] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x32;

    memory[0xFFFC] = Instruction::LDY_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x32;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x12;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDY_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x60;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x60;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDY_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0xA;
    const Word POSITION = 0x6060;

    memory[0xFFFC] = Instruction::LDY_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0xA8;
    const Byte VALUE = 0xA;
    const Word POSITION = 0xA122;

    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::LDY_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDY_Test, LDY_Absolute_OffsetX_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0xBC;
    const Byte VALUE = 0xA;
    const Word POSITION = 0x16EE;

    processor.RegX = OFFSET;
    
    memory[0xFFFC] = Instruction::LDY_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}