#include "../CPU_Model.h"

class ROL_Test : public M6502 {};

TEST_F(ROL_Test, ROL_RegA_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10010100;
    processor.RegA = VALUE;
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::ROL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b00101001);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(ROL_Test, ROL_RegA_Test_NegvFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b01010101;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROL_RGA;

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

TEST_F(ROL_Test, ROL_RegA_Test_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b00000000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROL_RGA;

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

TEST_F(ROL_Test, ROL_RegA_Test_RollOver)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE = 0b10000000;
    processor.RegA = VALUE;
    memory[0xFFFC] = Instruction::ROL_RGA;
    memory[0xFFFD] = Instruction::ROL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 1);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(ROL_Test, ROL_RegA_Test_CarryFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE = 0b10100001;
    processor.RegA = VALUE;
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::ROL_RGA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, 0b01000011);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}

TEST_F(ROL_Test, ROL_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE = 0b01011010;
    const Word POSITION = 0x43;
    processor.Flags.Carry = 1;

    memory[0xFFFC] = Instruction::ROL_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], 0b10110101);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROL_Test, ROL_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b10111010;
    const Word POSITION = 0x12;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ROL_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], 0b01110100);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROL_Test, ROL_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 12;
    const Byte VALUE = 0b10111010;
    const Word POSITION = 0xF2;

    processor.RegX = 0x32;
    memory[0xFFFC] = Instruction::ROL_ZPX;
    memory[0xFFFD] = POSITION;
    memory[0xFFFE] = Instruction::ROL_ZPX;
    memory[0xFFFF] = POSITION;
    memory[(Byte) (POSITION + processor.RegX)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSITION + processor.RegX)], 0b11101001);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}
TEST_F(ROL_Test, ROL_Absolute)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE = 0b10000000;
    const Word POSITION = 0x43F1;

    memory[0xFFFC] = Instruction::ROL_AB;
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
TEST_F(ROL_Test, ROL_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 7;
    const Byte VALUE = 0b00101110;
    const Word POSITION = 0x43F1;

    processor.RegX = 0xF1;
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::ROL_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], 0b01011101);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    FlagsExcept_NegvZeroCarry(processor);
}