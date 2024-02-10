#include "../CPU_Model.h"

class Register_Test : public M6502 {};

TEST_F(Register_Test, TAX)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x16;
    
    processor.Reset();
    processor.RegA = VALUE;

    memory[0xFFFC] = Instruction::TAX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, TAY)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x81;
    
    processor.Reset();
    processor.RegA = VALUE;

    memory[0xFFFC] = Instruction::TAY;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, TXA)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x17;

    processor.Reset();
    processor.RegX = VALUE;

    memory[0xFFFC] = Instruction::TXA;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, TYA)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x00;

    processor.Reset();
    processor.RegA = 0x10;
    processor.RegY = VALUE;

    memory[0xFFFC] = Instruction::TYA;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, DEX)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x11;

    processor.Reset();
    processor.RegX = VALUE;

    memory[0xFFFC] = Instruction::DEX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE - 1);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, DEY)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x00;

    processor.Reset();
    processor.RegY = VALUE;

    memory[0xFFFC] = Instruction::DEY;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, (Byte) (VALUE - 1));
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, INX)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = (Byte) -1;

    processor.Reset();
    processor.RegX = VALUE;

    memory[0xFFFC] = Instruction::INX;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, (Byte) (VALUE + 1));
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}
TEST_F(Register_Test, INY)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x64;

    processor.Reset();
    processor.RegY = VALUE;
    
    memory[0xFFFC] = Instruction::INY;

    // When
    uint32_t cycles_executed = processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegY, (Byte) (VALUE + 1));
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
}