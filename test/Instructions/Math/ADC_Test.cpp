#include "../CPU_Model.h"

class ADC_Test : public M6502 {};

TEST_F(ADC_Test, ADC_IM)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE_1 = 2;
    const Byte VALUE_2 = 3;
    
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Instruction::ADC_IM;
    memory[0xFFFD] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[0xFFFD], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}

TEST_F(ADC_Test, ADC_ZP)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte POSITION = 0x97;
    const Byte VALUE_1 = 20;
    const Byte VALUE_2 = 31;
    
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Instruction::ADC_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_ZPX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte POSITION = 0x20;
    const Byte OFFSET = 0x20;
    const Byte VALUE_1 = 70;
    const Byte VALUE_2 = 2;
    
    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION+OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION+OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}

TEST_F(ADC_Test, ADC_ZPX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte POSITION = 0x97;
    const Byte OFFSET = 0x99;
    const Byte VALUE_1 = 20;
    const Byte VALUE_2 = 81;
    
    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(POSITION+OFFSET) % 0x100] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[(POSITION+OFFSET) % 0x100], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_AB)
{
    // Given
    const uint32_t CYCLES = 4;
    const Word POSITION = 0xABCD;
    const Byte VALUE_1 = 100;
    const Byte VALUE_2 = 27;
    
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Instruction::ADC_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_ABX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Word POSITION = 0xCD01;
    const Byte OFFSET = 0x20;
    const Byte VALUE_1 = 17;
    const Byte VALUE_2 = 0;
    
    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_ABX_PageCross)
{
    // Given
    const uint32_t CYCLES = 5;
    const Word POSITION = 0xE1F9;
    const Byte OFFSET = 0xF1;
    const Byte VALUE_1 = 15;
    const Byte VALUE_2 = 15;
    
    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_ABY)
{
    // Given
    const uint32_t CYCLES = 4;
    const Word POSITION = 0xCD01;
    const Byte OFFSET = 0x20;
    const Byte VALUE_1 = 81;
    const Byte VALUE_2 = 16;
    
    processor.RegA = VALUE_1;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_ABY_PageCross)
{
    // Given
    const uint32_t CYCLES = 5;
    const Word POSITION = 0x00F1;
    const Byte OFFSET = 0x22;
    const Byte VALUE_1 = 72;
    const Byte VALUE_2 = 18;
    
    processor.RegA = VALUE_1;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ADC_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_IDX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION_1 = 0x0016;
    const Word POSITION_2 = 0xD04E;
    const Byte OFFSET = 0x20;
    const Byte VALUE_1 = 16;
    const Byte VALUE_2 = 32;
    
    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Instruction::ADC_IDX;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Word) POSITION_1 + OFFSET, POSITION_2);
    memory[POSITION_2] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION_2], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_IDY)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte POSITION_1 = 0x0098;
    const Word POSITION_2 = 0x040E;
    const Byte OFFSET = 0x64;
    const Byte VALUE_1 = 30;
    const Byte VALUE_2 = 2;
    
    processor.RegA = VALUE_1;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ADC_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Word) POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION_2 + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}
TEST_F(ADC_Test, ADC_IDY_PageCros)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte POSITION_1 = 0x0069;
    const Word POSITION_2 = 0x0420;
    const Byte OFFSET = 0xFF;
    const Byte VALUE_1 = 57;
    const Byte VALUE_2 = 39;
    
    processor.RegA = VALUE_1;
    processor.RegY = OFFSET;

    memory[0xFFFC] = Instruction::ADC_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Word) POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, (Byte) (VALUE_1 + VALUE_2));
    EXPECT_EQ(memory[POSITION_2 + OFFSET], VALUE_2);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
}