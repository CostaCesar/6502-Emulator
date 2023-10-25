#include "CPU_Model.h"

class Shift_Test : public M6502 {};

TEST_F(Shift_Test, ASL_RegA_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b00010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_NegvFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b01010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE << 1);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0b10000000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;
    memory[0xFFFD] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_TRUE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_RegA_Test_CarryFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10100000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ASL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE << 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(Shift_Test, ASL_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE = 0b01011010;
    const Word POSITION = 0x43;

    memory[0xFFFC] = Instruction::ASL_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], VALUE << 1);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(Shift_Test, ASL_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b00111010;
    const Word POSITION = 0x12;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ASL_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE << 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(Shift_Test, ASL_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b10111010;
    const Word POSITION = 0xF2;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ASL_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + processor.RegX)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSITION + processor.RegX)], (Byte) (VALUE << 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(Shift_Test, ASL_Absolute)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b10000000;
    const Word POSITION = 0x43F1;

    memory[0xFFFC] = Instruction::ASL_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], (Byte) (VALUE << 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_TRUE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(Shift_Test, ASL_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 7;
    const Byte VALUE = 0b0010111;
    const Word POSITION = 0x43F1;

    processor.RegX = 0xF1;
    memory[0xFFFC] = Instruction::ASL_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], (Byte) (VALUE << 1));
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}