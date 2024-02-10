#include "../CPU_Model.h"

class INC_DEC_Test : public M6502 {};

TEST_F(INC_DEC_Test, DEC_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte POSITION = 0x32;
    const Byte VALUE = 0x17;

    memory[0xFFFC] = Instruction::DEC_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], VALUE-1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, DEC_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0x12;
    const Byte VALUE = 0x29;

    processor.RegX = 0xA;

    memory[0xFFFC] = Instruction::DEC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION+processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE-1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, DEC_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0x64;
    const Byte VALUE = 0xDC;

    processor.RegX = 0xFF;

    memory[0xFFFC] = Instruction::DEC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + processor.RegX)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSITION + processor.RegX)], VALUE-1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, DEC_Absolute)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0x32;
    const Byte VALUE = 0x17;

    memory[0xFFFC] = Instruction::DEC_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], VALUE-1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, DEC_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 7;
    const Byte POSITION = 0x32;
    const Byte VALUE = 0x17;

    processor.RegX = 0xA1;

    memory[0xFFFC] = Instruction::DEC_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE-1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, INC_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte POSITION = 0x10;
    const Byte VALUE = 0xDE;

    memory[0xFFFC] = Instruction::INC_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], VALUE+1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, INC_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0xA4;
    const Byte VALUE = 0xF1;

    processor.RegX = 0xB;

    memory[0xFFFC] = Instruction::INC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION+processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE+1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, INC_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0x74;
    const Byte VALUE = 0xDF;

    processor.RegX = 0xFF;

    memory[0xFFFC] = Instruction::INC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + processor.RegX)] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[(Byte) (POSITION + processor.RegX)], VALUE+1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, INC_Absolute)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION = 0x30;
    const Byte VALUE = 0x01;

    memory[0xFFFC] = Instruction::INC_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION], VALUE+1);
    FlagsExcept_NegvZero(processor);
}

TEST_F(INC_DEC_Test, INC_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 7;
    const Byte POSITION = 0x64;
    const Byte VALUE = 0x07;

    processor.RegX = 0xA1;
    
    memory[0xFFFC] = Instruction::INC_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + processor.RegX] = VALUE;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[POSITION + processor.RegX], VALUE+1);
    FlagsExcept_NegvZero(processor);
}