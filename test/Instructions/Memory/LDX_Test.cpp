#include "../CPU_Model.h"

class LDX_Test : public M6502 {};

TEST_F(LDX_Test, LDX_Imediatate)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0xA;

    memory[0xFFFC] = Instruction::LDX_IM;
    memory[0xFFFD] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x10;

    memory[0xFFFC] = Instruction::LDX_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_ZeroPage_OffsetY)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x10;
    const Byte OFFSET = 0x5;

    processor.RegY = 0x5;

    memory[0xFFFC] = Instruction::LDX_ZPY;
    memory[0xFFFD] = POSITION;
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_ZeroPage_OffsetY_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0xA;
    const Byte POSITION = 0x80;
    const Byte OFFSET = 0xFF;

    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::LDX_ZPY;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0xA;
    const Word POSITION = 0x1010;

    memory[0xFFFC] = Instruction::LDX_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_Absolute_OffsetY)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x08;
    const Byte VALUE = 0xA;
    const Word POSITION = 0xAAED;

    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::LDX_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}

TEST_F(LDX_Test, LDX_Absolute_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte OFFSET = 0xED;
    const Byte VALUE = 0xA;
    const Word POSITION = 0x80DC;

    processor.RegY = OFFSET;
    
    memory[0xFFFC] = Instruction::LDX_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}