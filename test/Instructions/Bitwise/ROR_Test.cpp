#include "../CPU_Model.h"

class ROR_Test : public M6502 {};

TEST_F(ROR_Test, ROR_RegA_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROR_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE >> 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_RegA_Test_NegvFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10100001;
    processor.RegA = VALUE;
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::ROR_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b11010000);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_RegA_Test_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b00000001;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROR_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_TRUE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(ROR_Test, ROR_RegA_Test_CarryFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10100001;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROR_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE >> 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}


TEST_F(ROR_Test, ROR_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE = 0b01011010;
    const Word POSITION = 0x43;
    processor.Flags.Carry = 1;

    memory[0xFFFC] = Instruction::ROR_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], 0b10101101);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b00111010;
    const Word POSITION = 0x12;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ROR_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE >> 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b10111011;
    const Word POSITION = 0xF2;
    processor.Flags.Carry = 1;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ROR_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + processor.RegX)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSITION + processor.RegX)], 0b11011101);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_Absolute)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b00000001;
    const Word POSITION = 0x43F1;

    memory[0xFFFC] = Instruction::ROR_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], (Byte) (VALUE >> 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_TRUE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROR_Test, ROR_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 7;
    const Byte VALUE = 0b0010110;
    const Word POSITION = 0x43F1;

    processor.RegX = 0xF1;
    memory[0xFFFC] = Instruction::ROR_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], (Byte) (VALUE >> 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}